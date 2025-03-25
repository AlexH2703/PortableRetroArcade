#include "dbHelper.h"

bool dbHelper::connectDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={SQL Server};SERVER=localhost;DATABASE=PORTABLE_RETRO_ARCADE;Trusted_Connection=Yes;");

    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS PACMAN_HIGH_SCORES (id INTEGER PRIMARY KEY, INITAL TEXT, Score INTEGER)");

    return true;
}

QList<QPair<QString, int>> dbHelper::getTopHighScores(int limit) {
    QList<QPair<QString, int>> highScores;
    QString queryString = QString("SELECT TOP %1 INITAL, Score FROM PACMAN_HIGH_SCORES ORDER BY Score DESC").arg(limit);
    QSqlQuery query;

    if (query.exec(queryString)) {
        while (query.next()) {
            QString name = query.value(0).toString();
            int score = query.value(1).toInt();
            highScores.append(qMakePair(name, score));
        }
    }
    else {
        qDebug() << "Failed to retrieve high scores:" << query.lastError().text();
    }

    return highScores;
}

void dbHelper::addHighScore(const QString& name, int score) {
    QSqlQuery query;
    query.prepare("INSERT INTO PACMAN_HIGH_SCORES (INITAL, Score) VALUES (?, ?)");
    query.addBindValue(name);
    query.addBindValue(score);

    if (!query.exec()) {
        qDebug() << "Failed to add high score:" << query.lastError().text();
    }
}
