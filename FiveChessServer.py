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
        msgType, msg = data.split(' ', 1)
        if msgType == '/Login':
            username, password = msg.split(' ', 1)
            user = query(username)
            if user == ():
                data = '/UsernameNotExist'
            elif user[0][1] != password:
                data = '/PasswordError'
            elif user[0] in self.factory.player:
                data = '/UserHasLogged'
            else:
                data = str(user[0][0]) + ' ' + str(user[0][1]) + ' ' + str(user[0][2]) + ' ' + str(
                    user[0][3]) + ' ' + str(user[0][4]) + ' ' + str(user[0][5])
                self.factory.player.append(user[0])
                self.factory.status.append("/idle")
                self.factory.chessBoard.append(chessBoard())
                userList = "/UpdateUserList "
                for i in self.factory.player:
                    userList += str(i[2]) + ' '
                userList = userList[:-1]
                for client in self.factory.client:
                    client.transport.write(userList)
                self.factory.client.append(self)

        elif msgType == '/Register':
            username, password = msg.split(' ')
            user = query(username)
            if user:
                data = '/UsernameExist'
            else:
                insert(username, password, username)
                user = query(username)
                data = str(user[0][0]) + ' ' + str(user[0][1]) + ' ' + str(user[0][2]) + ' ' + str(
                    user[0][3]) + ' ' + str(user[0][4]) + ' ' + str(user[0][5])
                self.factory.player.append(user[0])
                self.factory.status.append("/idle")
                self.factory.chessBoard.append(chessBoard())
                userList = "/UpdateUserList "
                for i in self.factory.player:
                    userList += str(i[2]) + ' '
                userList = userList[:-1]
                for client in self.factory.client:
                    client.transport.write(userList)
                self.factory.client.append(self)

        elif msgType == '/GetUserList':
            data = "/UpdateUserList "
            for i in self.factory.player:
                data += str(i[2]) + ' '
            data = data[:-1]

        elif msgType == '/GetUserInfo':
            user = query(msg)
            data = "/UserInfo " + str(user[0][2]) + ' ' + str(user[0][3]) + ' ' + str(user[0][4]) + ' ' + str(
                user[0][5])

        elif msgType == '/SendMsg':
            for client in self.factory.client:
                client.transport.write(data)
            return

        elif msgType == '/AlterName':
            user = query2(msg)
            if user:
                data = '/AlterNameError'
            else:
                username, password, data, win, lose, draw = self.factory.player[self.factory.client.index(self)]
                update(username, password, msg, win, lose, draw)
                self.factory.player[self.factory.client.index(self)] = (username, password, msg, win, lose, draw)
                data = '/AlterNameSuccess ' + msg
                userList = "/UpdateUserList "
                for i in self.factory.player:
                    userList += str(i[2]) + ' '
                userList = userList[:-1]
                for client in self.factory.client:
                    if client != self:
                        client.transport.write(userList)

        elif msgType == '/AlterPasswd':
            username, data, nickname, win, lose, draw = self.factory.player[self.factory.client.index(self)]
            update(username, msg, nickname, win, lose, draw)
            self.factory.player[self.factory.client.index(self)] = (username, msg, nickname, win, lose, draw)
            data = '/AlterPasswdSuccess ' + msg

        elif msgType == '/Invite':
            user = query2(msg)
            if self.factory.status[self.factory.player.index(user[0])] == "/idle":
                self.factory.client[self.factory.player.index(user[0])].transport.write(
                    "/Invited " + str(self.factory.player[self.factory.client.index(self)][2]))
                return
            else:
                data = '/InviteFailed 0'

        elif msgType == '/InviteSuccess':
            user = query2(msg)
            self.factory.status[self.factory.player.index(user[0])] = self.factory.player[
                self.factory.client.index(self)]
            self.factory.status[self.factory.client.index(self)] = user[0]
            for i in range(225):
                self.factory.chessBoard[self.factory.player.index(user[0])][i] = 255
                self.factory.chessBoard[self.factory.client.index(self)][i] = 255
            self.factory.client[self.factory.player.index(user[0])].transport.write("/InviteSuccess 1")
            return

        elif msgType == '/InviteFailed':
            user = query2(msg)
            self.factory.client[self.factory.player.index(user[0])].transport.write("/InviteFailed 1")
            return

        elif msgType == '/BeginWithAI':
            for i in range(225):
                self.factory.chessBoard[self.factory.client.index(self)][i] = 255
            self.factory.chessBoard[self.factory.client.index(self)][112] = 0
            self.factory.status[self.factory.client.index(self)] = "/AI"
            data = "/NextStep 7 7"

        elif msgType == '/NextStep':
            x, y, chessType = msg.split(' ')
            self.factory.chessBoard[self.factory.client.index(self)][int(y) * 15 + int(x)] = int(chessType)
            user = self.factory.status[self.factory.client.index(self)]
            if rule.GetIsWin(self.factory.chessBoard[self.factory.client.index(self)], int(x), int(y)):
                data = "/Win"
                if user != "/AI":
                    self.factory.client[self.factory.player.index(user)].transport.write(
                        "/Lose " + str(x) + " " + str(y))
                    self.factory.status[self.factory.player.index(user)] = "/idle"
                self.factory.status[self.factory.client.index(self)] = "/idle"
            elif user == "/AI":
                nextStep = ai.GetAGoodMove(self.factory.chessBoard[self.factory.client.index(self)], 3, 1)
                a = nextStep / 100
                b = nextStep - 100 * a
                self.factory.chessBoard[self.factory.client.index(self)][b * 15 + a] = 0
                if rule.GetIsWin(self.factory.chessBoard[self.factory.client.index(self)], a, b):
                    data = "/Lose " + str(a) + " " + str(b)
                    self.factory.status[self.factory.client.index(self)] = "/idle"
                else:
                    data = "/NextStep " + str(a) + " " + str(b)
            else:
                self.factory.chessBoard[self.factory.player.index(user)][int(y) * 15 + int(x)] = int(chessType)
                self.factory.client[self.factory.player.index(user)].transport.write(
                    "/NextStep " + str(x) + " " + str(y))
                return

        elif msgType == '/DrawChess':
            user = self.factory.status[self.factory.client.index(self)]
            if user == "/AI":
                self.factory.status[self.factory.client.index(self)] = "/idle"
                data = "/Draw"
            else:
                self.factory.client[self.factory.player.index(user)].transport.write("/IsDraw")
                return

        elif msgType == '/Draw':
            user = self.factory.status[self.factory.client.index(self)]
            self.factory.client[self.factory.player.index(user)].transport.write("/Draw")
            self.factory.status[self.factory.player.index(user)] = "/idle"
            self.factory.status[self.factory.client.index(self)] = "/idle"
            return

        elif msgType == '/NoDraw':
            user = self.factory.status[self.factory.client.index(self)]
            self.factory.client[self.factory.player.index(user)].transport.write("/NoDraw")
            return

        elif msgType == '/Surrender':
            user = self.factory.status[self.factory.client.index(self)]
            if user != "/AI":
                self.factory.client[self.factory.player.index(user)].transport.write("/Win")
                self.factory.status[self.factory.player.index(user)] = "/idle"
            self.factory.status[self.factory.client.index(self)] = "/idle"
            return

        elif msgType == '/Logout':
            del self.factory.player[self.factory.client.index(self)]
            del self.factory.status[self.factory.client.index(self)]
            del self.factory.chessBoard[self.factory.client.index(self)]
            self.factory.client.remove(self)
            userList = "/UpdateUserList "
            for i in self.factory.player:
                userList += str(i[2]) + ' '
            userList = userList[:-1]
            for client in self.factory.client:
                client.transport.write(userList)
            return

        self.transport.write(data)


class ChessFactory(Factory):
    protocol = ChessServer

    def __init__(self):
        self.client = []
        self.player = []
        self.status = []
        self.chessBoard = []


reactor.listenTCP(7110, ChessFactory())
reactor.run()


