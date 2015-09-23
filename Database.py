__author__ = 'fate'

import MySQLdb


def query(username):
    conn = MySQLdb.connect(host="localhost", user="root", passwd="123456", db="FiveChess", charset="utf8")
    cursor = conn.cursor()
    sqlStr = "select * from User where UserName='" + username + "'"
    cursor.execute(sqlStr)
    data = cursor.fetchall()
    cursor.close()
    conn.close()
    return data


def query2(nickname):
    conn = MySQLdb.connect(host="localhost", user="root", passwd="123456", db="FiveChess", charset="utf8")
    cursor = conn.cursor()
    sqlStr = "select * from User where NickName='" + nickname + "'"
    cursor.execute(sqlStr)
    data = cursor.fetchall()
    cursor.close()
    conn.close()
    return data


def insert(username, password, nickname):
    conn = MySQLdb.connect(host="localhost", user="root", passwd="123456", db="FiveChess", charset="utf8")
    cursor = conn.cursor()
    sqlStr = "insert into User values('" + username + "','" + password + "','" + nickname + "',0,0,0)"
    cursor.execute(sqlStr)
    conn.commit()
    cursor.close()
    conn.close()


def delete(username):
    conn = MySQLdb.connect(host="localhost", user="root", passwd="123456", db="FiveChess", charset="utf8")
    cursor = conn.cursor()
    sqlStr = "delete from User where UserName='" + username + "'"
    cursor.execute(sqlStr)
    conn.commit()
    cursor.close()
    conn.close()


def update(username, password, nickname, win, lose, draw):
    conn = MySQLdb.connect(host="localhost", user="root", passwd="123456", db="FiveChess", charset="utf8")
    cursor = conn.cursor()
    sqlStr = "update User set PassWord = '" + password + "', NickName = '" + nickname + "', Win = " + str(
        win) + ", Lose = " + str(lose) + ", Draw = " + str(draw) + " where UserName = '" + username + "'"
    cursor.execute(sqlStr)
    conn.commit()
    cursor.close()
    conn.close()


