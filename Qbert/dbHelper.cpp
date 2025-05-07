#include "dbHelper.h"

bool dbHelper::connectDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={SQL Server};SERVER=localhost;DATABASE=PORTABLE_RETRO_ARCADE;Trusted_Connection=Yes;");

    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;

    // Create table for Qbert high scores if it doesn't exist
    query.exec(R"(
    IF NOT EXISTS (SELECT * FROM sysobjects WHERE name='QBERT_HIGH_SCORES' AND xtype='U')
    CREATE TABLE QBERT_HIGH_SCORES (
        INITAL VARCHAR(10),
        Score INT,
        DATE_ACHIVED DATETIME DEFAULT GETDATE(),
    )
)");

    return true;
}

QList<QPair<QString, int>> dbHelper::getTopHighScores(int limit) {
    QList<QPair<QString, int>> highScores;

    // Build query safely
    QString queryString = QString("SELECT TOP %1 INITAL, Score FROM QBERT_HIGH_SCORES ORDER BY Score DESC")
        .arg(QString::number(limit));

    QSqlQuery query;
    if (query.exec(queryString)) {
        while (query.next()) {
            QString name = query.value(0).toString();
            int score = query.value(1).toInt();

            qDebug() << "High Score Retrieved:" << name << score;

            highScores.append(QPair<QString, int>(name, score));
        }
    }
    else {
        qDebug() << "Failed to retrieve high scores:" << query.lastError().text();
        qDebug() << "Query was:" << queryString;
    }

    return highScores;
}


void dbHelper::addHighScore(const QString& name, int score) {
    QSqlQuery query;
    query.prepare("INSERT INTO QBERT_HIGH_SCORES (INITAL, Score) VALUES (?, ?)");
    query.addBindValue(name);
    query.addBindValue(score);

    if (!query.exec()) {
        qDebug() << "Failed to add high score:" << query.lastError().text();
    }
}
