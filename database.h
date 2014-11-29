#pragma once
#include <sqlite3.h>
#include <iostream>
#include "experiment.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree;

// Database contains experiment data.
class Database
{
public:
    Database(const string& databaseName)
    {
	db = NULL;
	int rc = sqlite3_open_v2(databaseName.c_str(), &db, SQLITE_OPEN_READONLY, NULL);
	if (rc != SQLITE_OK) 
	{
	    cerr << "cannot open database " << rc << endl;
	}
    }

    virtual ~Database()
    {
	if (db != NULL)
	{
	    sqlite3_close(db);
	}
    }

    virtual Experiment* FindExperiment(int ix)
    {

	Experiment* result = new Experiment();
	result->SetIx(ix);
	char* sql = sqlite3_mprintf("SELECT t, d FROM state WHERE ix_experiment=%d ORDER BY t", ix);
	sqlite3_stmt * stmt = NULL;
	int rc = sqlite3_prepare_v2 (db, sql, strlen (sql) + 1, & stmt, NULL);
	if (rc != SQLITE_OK) 
	{
	    cerr << "cannot open statement " << rc << endl;
	    return NULL;
	}
	for (int row=0;;row++) {
	    int rc = sqlite3_step (stmt);
	    if (rc == SQLITE_ROW) {
		int time = 0;
		const char * data = NULL;
		time = sqlite3_column_int(stmt, 0);
		data  = (const char*)sqlite3_column_text (stmt, 1);
		State* s = NewState(time, data);

		if (row == 0) 
		{
		    result->SetId(s->GetId());
		}
		result->AddState(time, *s);
	    }
	    else if (rc == SQLITE_DONE) {
		break;
	    }
	    else 
	    {
		cerr << "cannot continue statement " << rc << endl;
		return NULL;
	    }
	}
	return result;
    }

protected:
    sqlite3* db;

    State* NewState(int time, const string& jsonData)
    {
	State* result = new State();
	result->SetTime(time);

        ptree pt;
        stringstream ss; ss << jsonData;
        read_json(ss, pt);
	result->SetId(pt.get<string>("id"));

	string sss = pt.get<string>("s");
	result->SetPhase(State::String2Phase(sss));

	return result;
    }
    
};
