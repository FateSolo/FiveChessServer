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
            "/NextStep": self.next_step,
            "/DrawChess": self.draw_chess,
            "/Draw": self.draw,
            "/NoDraw": self.no_draw,
            "/Surrender": self.surrender,
            "/Logout": self.logout
        }

        self.client = {}
        self.nickname = {}
        self.username = {}

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
        self.send_to_all("/SendMsg " + msg)

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

            user_list = self.update_user_list()
            self.send_to_all(user_list)

    def alter_password(self, client, msg):
        username = self.client[client]
        update(username, msg, None, None, None, None)

        self.username[username]["password"] = msg

        self.send_to_client(client, "/AlterPasswordSuccess " + msg)

    def invite(self, client, msg):
        user = self.get_user(None, msg)

        if user["status"] == "/idle":
            self.send_to_client(user["client"], "/Invited " + self.get_user(client, None)["nickname"])

        else:
            self.send_to_client(client, "/InviteFailed 0")

    def invite_success(self, client, msg):
        user = self.get_user(client, None)
        user2 = self.get_user(None, msg)

        user["status"] = user2["nickname"]
        user2["status"] = user["nickname"]

        for i in range(225):
            user["chessBoard"][i] = 255
            user2["chessBoard"][i] = 255

        self.send_to_client(user2["client"], "/InviteSuccess 1")

    def invite_failed(self, client, msg):
        user = self.get_user(None, msg)

        self.send_to_client(user["client"], "/InviteFailed 1")

    def begin_with_ai(self, client, msg):
        user = self.get_user(client, None)

        for i in range(225):
            user["chessBoard"][i] = 255
        user["chessBoard"][112] = 0
        user["status"] = "/AI"

        self.send_to_client(client, "/NextStep 7 7")

    def next_step(self, client, msg):
        x, y, chessType = msg.split(' ')
        user = self.get_user(client, None)
        user["chessBoard"][int(y) * 15 + int(x)] = int(chessType)

        if user["status"] == "/AI":
            if rule.GetIsWin(user["chessBoard"], int(x), int(y)):
                self.win_update(user)

            else:
                nextStep = ai.GetAGoodMove(user["chessBoard"], 3)
                a = nextStep / 100
                b = nextStep - 100 * a
                user["chessBoard"][b * 15 + a] = 0

                if rule.GetIsWin(user["chessBoard"], a, b):
                    self.lose_update(user, str(a), str(b), True)

                else:
                    self.send_to_client(client, "/NextStep " + str(a) + " " + str(b))

        else:
            user2 = self.get_user(None, user["status"])

            if rule.GetIsWin(user["chessBoard"], int(x), int(y)):
                self.win_update(user)
                self.lose_update(user2, x, y, True)

            else:
                user2["chessBoard"][int(y) * 15 + int(x)] = int(chessType)
                self.send_to_client(user2["client"], "/NextStep " + x + " " + y)

    def draw_chess(self, client, msg):
        user = self.get_user(client, None)

        if user["status"] == "/AI":
            self.draw_update(user, True)

        else:
            self.send_to_client(self.get_user(None, user["status"])["client"], "/IsDraw")

    def draw(self, client, msg):
        user = self.get_user(client, None)
        user2 = self.get_user(None, user["status"])

        self.draw_update(user, False)
        self.draw_update(user2, True)

    def no_draw(self, client, msg):
        user = self.get_user(None, self.get_user(client, None)["status"])

        self.send_to_client(user["client"], "/NoDraw")

    def surrender(self, client, msg):
        user = self.get_user(client, None)

        if user["status"] != "/AI":
            user2 = self.get_user(None, user["status"])

            self.win_update(user2)

        self.lose_update(user, '0', '0', False)

    def logout(self, client, msg):
        user = self.get_user(client, None)

        if user["status"] != "/idle" and user["status"] != "/AI":
            user2 = self.get_user(None, user["status"])

            self.win_update(user2)
            self.lose_update(user, '0', '0', False)

        del self.client[client]
        del self.nickname[user["nickname"]]
        del self.username[user["username"]]

        user_list = self.update_user_list()
        self.send_to_all(user_list)

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
            user_list += nickname + ' '

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
            "level": 0,
            "chessBoard": chessBoard()
        }

        user_list = self.update_user_list()
        self.send_to_all(user_list)

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


reactor.listenTCP(7110, ChessFactory())
reactor.run()
