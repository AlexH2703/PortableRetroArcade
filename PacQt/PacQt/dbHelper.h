#ifndef DBHELPER_H
#define DBHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>
#include <QPair>

class dbHelper {
public:
    static bool connectDatabase();
    static QList<QPair<QString, int>> getTopHighScores(int limit);
    static void addHighScore(const QString& name, int score);
};

#endif // DBHELPER_H
