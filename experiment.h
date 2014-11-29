#pragma once
#include "state.h"


// Experiment data.
class Experiment
{
public:
Experiment(): ix(0) {}

    virtual ~Experiment()
    {}

    // Get Unix time of experiment start. Zero or negative means that the
    // experiment never went from prepare to running state.
    int GetStartTime() const
    {
	for(int i = 0;  i < states.size(); i++)
	{
	    if (states[i].GetPhase() == State::run)
	    {
		return states[i].GetTime();
	    }
	}
	return 0;
    }

    int GetIx() const
    { return ix; }

    string GetId() const
    { return id; }

    void SetIx(int value)
    { ix = value; }
    
    void SetId(const string& value)
    { id = value; }

    const State* FindNearestState(int time) const
    {
	for(int i = 0;  i < states.size(); i++)
	{
	    int t = states[i].GetTime();
	    if (t > time) 
	    {
		if (i == 0) return &states[0];
		return &states[i-1];
	    }

	}
	return NULL;
    }

    void AddState(int time, const State& state)
    { states.push_back(state); }

protected:
    int ix;
    string id;

    vector<State> states;
    
    
};
