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

	string stringPhase = pt.get<string>("s");
	result->SetStringPhase(stringPhase);
	result->SetPhase(State::String2Phase(stringPhase));
	
	result->SetProfile(pt.get<string>("pr"));
	result->SetTc(pt.get<string>("Tc"));
	result->SetTd(pt.get<string>("Td"));
	result->SetTf(pt.get<string>("Tf"));
	result->SetTb(pt.get<string>("Tb"));
	result->SetTw(pt.get<string>("Tw"));
	result->SetTo(pt.get<string>("To"));
	result->SetTp(pt.get<string>("Tp"));


	result->SetFr(pt.get<string>("fr"));
	result->SetFt(pt.get<string>("ft"));
	result->SetHr(pt.get<string>("hr"));
	result->SetHt(pt.get<string>("ht"));

	return result;
    }
    
};
