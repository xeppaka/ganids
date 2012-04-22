#include "db.h"
#include <iostream>

using namespace std;

Db::Db():
    db(NULL)
{
}

Db::~Db()
{
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}

bool Db::open(const char *filename)
{
    int rc;
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc)
        return false;

    char *zErrMsg = NULL;
    rc = sqlite3_exec(db, "create table if not exists nids_log (timestamp TEXT, action TEXT, message TEXT, rule TEXT)", NULL, 0, &zErrMsg);
    close();
    if (rc)
        return false;

    return true;
}

void Db::close()
{
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}

bool Db::insert(const char *action, const char *message, const char *rule)
{
    cout << "inserting to db" << endl;
    if (!db)
        return false;

    sqlite3_stmt *stmt = NULL;
    int rc = 0;
    try {
        rc = sqlite3_prepare_v2(db, "insert into nids_log values (datetime('now'), ?, ?, ?)", -1, &stmt, NULL);
        if (rc != SQLITE_OK)
            throw 1;

        rc = sqlite3_bind_text(stmt, 1, action, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK)
            throw 1;

        rc = sqlite3_bind_text(stmt, 2, message, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK)
            throw 1;

        rc = sqlite3_bind_text(stmt, 3, rule, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK)
            throw 1;

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE)
            throw 1;
    } catch (int i)
    {
        cout << "error inserting to db" << endl;
        if (stmt)
            sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool Db::is_opened()
{
    return db != NULL;
}
