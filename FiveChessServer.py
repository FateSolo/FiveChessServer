__author__ = 'Fate'

import sys

reload(sys)
sys.setdefaultencoding('utf8')

from twisted.internet.protocol import Factory, Protocol
from twisted.internet import reactor
from Database import *
from ctypes import *

ai = cdll.LoadLibrary("./FiveChessAI.so")
rule = cdll.LoadLibrary("./FiveChessRule.so")
chessBoard = c_ubyte * 225


class ChessServer(Protocol):
    def dataReceived(self, data):
        msg_type, msg = data.split(' ', 1)

        self.factory.handler[msg_type](self, msg)


class ChessFactory(Factory):
    protocol = ChessServer

    def __init__(self):
        self.handler = {
            "/Login": self.login,
            "/Register": self.register,
            "/GetUserList": self.get_user_list,
            "/GetUserInfo": self.get_user_info,
            "/SendMsg": self.send_msg,
            "/AlterName": self.alter_name,
            "/AlterPassword": self.alter_password,
            "/Invite": self.invite,
            "/InviteSuccess": self.invite_success,
            "/InviteFailed": self.invite_failed,
            "/BeginWithAI": self.begin_with_ai,
            "/BeginWithOther": self.begin_with_other,
            "/StopWithOther": self.stop_with_other,
            "/NextStep": self.next_step,
            "/DrawChess": self.draw_chess,
            "/Draw": self.draw,
            "/NoDraw": self.no_draw,
            "/Surrender": self.surrender,
            "/RetractChess": self.retract_chess,
            "/Retract": self.retract,
            "/NoRetract": self.no_retract,
            "/Logout": self.logout
        }

        self.client = {}
        self.nickname = {}
        self.username = {}

        self.waiting = "/null"

    def login(self, client, msg):
        username, password = msg.split(' ')
        user = query(username, None)

        if user == ():
            self.send_to_client(client, "/UsernameNotExist")

        elif user[0][1] != password:
            self.send_to_client(client, "/PasswordError")

        elif user[0][0] in self.username.keys():
            self.send_to_client(client, "/UserHasLogged")

        else:
            self.add_user(client, user[0])

    def register(self, client, msg):
        username, password = msg.split(' ')
        user = query(username, None)

        if user:
            self.send_to_client(client, "/UsernameExist")

        else:
            insert(username, password, username)
            user = query(username, None)

            self.add_user(client, user[0])

    def get_user_list(self, client, msg):
        self.send_to_client(client, self.update_user_list())

    def get_user_info(self, client, msg):
        user = self.get_user(None, msg)
        msg = "/UserInfo " + user["nickname"] + ' ' + str(user["win"]) + ' ' + str(user["lose"]) + ' ' + str(
            user["draw"])

        self.send_to_client(client, msg)

    def send_msg(self, client, msg):
        user = self.get_user(client, None)
        msg = "/SendMsg " + msg

        if user["status"] == "/idle" or user["status"] == "/waiting":
            for value in self.username.values():
                if value["status"] == "/idle" or user["status"] == "/waiting":
                    self.send_to_client(value["client"], msg)

        else:
            if user["status"] != "/AI":
                self.send_to_client(self.get_user(None, user["status"])["client"], msg)

            self.send_to_client(client, msg)

    def alter_name(self, client, msg):
        user = query(None, msg)

        if user:
            self.send_to_client(client, "/AlterNameError")

        else:
            username = self.client[client]
            nickname = self.username[username]["nickname"]
            update(username, None, msg, None, None, None)

            del self.nickname[nickname]
            self.nickname[msg] = username
            self.username[username]["nickname"] = msg

            self.send_to_client(client, "/AlterNameSuccess " + msg)

            self.send_to_all(self.update_user_list())

    def alter_password(self, client, msg):
        username = self.client[client]
        update(username, msg, None, None, None, None)

        self.username[username]["password"] = msg

        self.send_to_client(client, "/AlterPasswordSuccess " + msg)

    def invite(self, client, msg):
        user = self.get_user(None, msg)

        if user["status"] == "/idle":
            self.send_to_client(user["client"], "/Invited " + self.get_user(client, None)["nickname"])

        elif user["status"] == "/waiting":
            self.send_to_client(client, "/InviteFailed 1")

        else:
            self.send_to_client(client, "/InviteFailed 0")

    def invite_success(self, client, msg):
        user = self.get_user(client, None)
        user2 = self.get_user(None, msg)

        if user["status"] != "/idle":
            self.send_to_client(client, "/InviteFailed 3")

        elif user2["status"] != "/idle":
            self.send_to_client(user2["client"], "/InviteFailed 4")

        else:
            user["status"] = user2["nickname"]
            user2["status"] = user["nickname"]

            for i in range(225):
                user["chessBoard"][i] = 255
                user2["chessBoard"][i] = 255

            user["x"] = -1
            user["y"] = -1

            user2["x"] = -1
            user2["y"] = -1

            self.send_to_client(user2["client"], "/InviteSuccess " + user["nickname"])

            self.send_to_all(self.update_user_list())

    def invite_failed(self, client, msg):
        user = self.get_user(None, msg)

        self.send_to_client(user["client"], "/InviteFailed 2")

    def begin_with_ai(self, client, msg):
        user = self.get_user(client, None)

        for i in range(225):
            user["chessBoard"][i] = 255
        user["chessBoard"][112] = 0

        user["x"] = -1
        user["y"] = -1

        user["status"] = "/AI"
        user["level"] = int(msg)

        self.send_to_client(client, "/NextStep 7 7")

        self.send_to_all(self.update_user_list())

    def begin_with_other(self, client, msg):
        user = self.get_user(client, None)

        if self.waiting == "/null":
            user["status"] = "/waiting"
            self.waiting = user["nickname"]

        else:
            user2 = self.get_user(None, self.waiting)

            user["status"] = user2["nickname"]
            user2["status"] = user["nickname"]

            for i in range(225):
                user["chessBoard"][i] = 255
                user2["chessBoard"][i] = 255

            user["x"] = -1
            user["y"] = -1

            user2["x"] = -1
            user2["y"] = -1

            self.send_to_client(user["client"], "/BeginWithOther " + user2["nickname"] + " 1")
            self.send_to_client(user2["client"], "/BeginWithOther " + user["nickname"] + " 0")

            self.waiting = "/null"

        self.send_to_all(self.update_user_list())

    def stop_with_other(self, client, msg):
        self.get_user(client, None)["status"] = "/idle"
        self.waiting = "/null"

        self.send_to_all(self.update_user_list())

    def next_step(self, client, msg):
        x, y, chess_type = msg.split(' ')
        user = self.get_user(client, None)
        user["chessBoard"][int(y) * 15 + int(x)] = int(chess_type)

        user["x"] = int(x)
        user["y"] = int(y)

        if user["status"] == "/AI":
            if rule.GetIsWin(user["chessBoard"], int(x), int(y)):
                self.win_update(user)

                self.send_to_all(self.update_user_list())

            else:
                next_step = ai.GetAGoodMove(user["chessBoard"], user["level"])
                a = next_step / 100
                b = next_step - 100 * a
                user["chessBoard"][b * 15 + a] = 0

                if rule.GetIsWin(user["chessBoard"], a, b):
                    self.lose_update(user, str(a), str(b), True)

                    self.send_to_all(self.update_user_list())

                else:
                    user["enemy_x"] = a
                    user["enemy_y"] = b

                    self.send_to_client(client, "/NextStep " + str(a) + " " + str(b))

        else:
            user2 = self.get_user(None, user["status"])

            if rule.GetIsWin(user["chessBoard"], int(x), int(y)):
                self.win_update(user)
                self.lose_update(user2, x, y, True)

                self.send_to_all(self.update_user_list())

            else:
                user2["enemy_x"] = int(x)
                user2["enemy_y"] = int(y)

                user2["chessBoard"][int(y) * 15 + int(x)] = int(chess_type)
                self.send_to_client(user2["client"], "/NextStep " + x + " " + y)

    def draw_chess(self, client, msg):
        user = self.get_user(client, None)

        if user["status"] == "/AI":
            self.draw_update(user, True)

            self.send_to_all(self.update_user_list())

        else:
            self.send_to_client(self.get_user(None, user["status"])["client"], "/IsDraw")

    def draw(self, client, msg):
        user = self.get_user(client, None)
        user2 = self.get_user(None, user["status"])

        self.draw_update(user, False)
        self.draw_update(user2, True)

        self.send_to_all(self.update_user_list())

    def no_draw(self, client, msg):
        user = self.get_user(None, self.get_user(client, None)["status"])

        self.send_to_client(user["client"], "/NoDraw")

    def surrender(self, client, msg):
        user = self.get_user(client, None)

        if user["status"] != "/AI":
            user2 = self.get_user(None, user["status"])

            self.win_update(user2)

        self.lose_update(user, '0', '0', False)

        self.send_to_all(self.update_user_list())

    def retract_chess(self, client, msg):
        user = self.get_user(client, None)

        if user["x"] == -1 or user["y"] == -1:
            self.send_to_client(user["client"], "/NoRetract 0")

        elif user["x"] == -2 or user["y"] == -2:
            self.send_to_client(user["client"], "/NoRetract 1")

        elif user["status"] == "/AI":
            self.retract_update(user, True)

        else:
            self.send_to_client(self.get_user(None, user["status"])["client"], "/IsRetract " + str(user["x"]) + ' ' +
                                str(user["y"]) + ' ' + str(user["enemy_x"]) + ' ' + str(user["enemy_y"]))

    def retract(self, client, msg):
        user = self.get_user(client, None)
        user2 = self.get_user(None, user["status"])

        self.retract_update(user, False)
        self.retract_update(user2, True)

    def no_retract(self, client, msg):
        user = self.get_user(None, self.get_user(client, None)["status"])

        self.send_to_client(user["client"], "/NoRetract 2")

    def logout(self, client, msg):
        user = self.get_user(client, None)

        if user["status"] != "/idle" and user["status"] != "/AI" and user["status"] != "/waiting":
            user2 = self.get_user(None, user["status"])

            self.win_update(user2)
            self.lose_update(user, '0', '0', False)

        del self.client[client]
        del self.nickname[user["nickname"]]
        del self.username[user["username"]]

        self.send_to_all(self.update_user_list())

    def send_to_client(self, client, msg):
        length = "%04d" % len(msg)
        msg = length + msg
        client.transport.write(msg)

    def send_to_all(self, msg):
        for client in self.client.keys():
            self.send_to_client(client, msg)

    def update_user_list(self):
        user_list = "/UpdateUserList "
        for nickname in self.nickname.keys():
            user_list += nickname + ' ' + self.get_user(None, nickname)["status"] + ' '

        return user_list[:-1]

    def add_user(self, client, user):
        username = str(user[0])
        password = str(user[1])
        nickname = str(user[2])
        win = int(user[3])
        lose = int(user[4])
        draw = int(user[5])

        msg = username + ' ' + password + ' ' + nickname + ' ' + str(win) + ' ' + str(lose) + ' ' + str(draw)
        self.send_to_client(client, msg)

        self.nickname[nickname] = username
        self.username[username] = {
            "client": client,
            "username": username,
            "password": password,
            "nickname": nickname,
            "win": win,
            "lose": lose,
            "draw": draw,
            "status": "/idle",
            "chessBoard": chessBoard(),
        }

        self.send_to_all(self.update_user_list())

        self.client[client] = username

    def get_user(self, client, nickname):
        if client is not None:
            return self.username[self.client[client]]

        elif nickname is not None:
            return self.username[self.nickname[nickname]]

        else:
            return None

    def win_update(self, user):
        if user["status"] != "/AI":
            user["win"] += 1
            update(user["username"], None, None, user["win"], None, None)

        user["status"] = "/idle"
        self.send_to_client(user["client"], "/Win")

    def lose_update(self, user, x, y, is_send):
        if user["status"] != "/AI":
            user["lose"] += 1
            update(user["username"], None, None, None, user["lose"], None)

        user["status"] = "/idle"

        if is_send:
            self.send_to_client(user["client"], "/Lose " + x + ' ' + y)

    def draw_update(self, user, is_send):
        if user["status"] != "/AI":
            user["draw"] += 1
            update(user["username"], None, None, None, None, user["draw"])

        user["status"] = "/idle"

        if is_send:
            self.send_to_client(user["client"], "/Draw")

    def retract_update(self, user, is_send):
        user["chessBoard"][user["y"] * 15 + user["x"]] = 255
        user["chessBoard"][user["enemy_y"] * 15 + user["enemy_x"]] = 255

        if is_send:
            self.send_to_client(user["client"], "/Retract " + str(user["x"]) + ' ' + str(user["y"]) + ' ' + str(
                user["enemy_x"]) + ' ' + str(user["enemy_y"]))

        user["x"] = -2
        user["y"] = -2


reactor.listenTCP(7110, ChessFactory())
reactor.run()
