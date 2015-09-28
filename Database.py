__author__ = 'fate'

import MySQLdb


def query(username, nickname):
    conn = MySQLdb.connect(host="localhost", user="root", passwd="123456", db="FiveChess", charset="utf8")
    cursor = conn.cursor()

    sqlStr = "select * from User where 1 = 1"
    args = []

    if username is not None:
        sqlStr += " and UserName = %s"
        args.append(username)

    if nickname is not None:
        sqlStr += " and NickName = %s"
        args.append(nickname)

    cursor.execute(sqlStr, args)
    data = cursor.fetchall()

    cursor.close()
    conn.close()

    return data


def insert(username, password, nickname):
    conn = MySQLdb.connect(host="localhost", user="root", passwd="123456", db="FiveChess", charset="utf8")
    cursor = conn.cursor()

    sqlStr = "insert into User values (%s, %s, %s, 0, 0, 0)"
    args = [username, password, nickname]

    cursor.execute(sqlStr, args)
    conn.commit()

    cursor.close()
    conn.close()


def delete(username):
    conn = MySQLdb.connect(host="localhost", user="root", passwd="123456", db="FiveChess", charset="utf8")
    cursor = conn.cursor()

    sqlStr = "delete from User where UserName= %s"
    args = [username]

    cursor.execute(sqlStr, args)
    conn.commit()

    cursor.close()
    conn.close()


def update(username, password, nickname, win, lose, draw):
    conn = MySQLdb.connect(host="localhost", user="root", passwd="123456", db="FiveChess", charset="utf8")
    cursor = conn.cursor()

    sqlStr = "update User set"
    args = []

    tmp = ""

    if password is not None:
        tmp += " PassWord = %s,"
        args.append(password)

    if nickname is not None:
        tmp += " NickName = %s,"
        args.append(nickname)

    if win is not None:
        tmp += " Win = %s,"
        args.append(win)

    if lose is not None:
        tmp += " Lose = %s,"
        args.append(lose)

    if draw is not None:
        tmp += " Draw = %s,"
        args.append(draw)

    if tmp != "":
        tmp = tmp[:-1]
        tmp += " where UserName = %s"
        args.append(username)

        cursor.execute(sqlStr, args)
        conn.commit()

    cursor.close()
    conn.close()
