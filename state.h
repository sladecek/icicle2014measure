#pragma once

// Physical variables at certain time.
class State
{
public:
    enum Phase { prepare, run, abort };

State():
    time(0), phase(prepare) {}

    virtual ~State() 
    {}

    int GetTime() const
    { return time; }

    void SetTime(int value) 
    { time = value; }

    string GetId() const 
    { return id; }

    void SetId(const string& value)
    { id = value; }

    string GetPrintableDate() const
    {  return printableDate; }

    string GetPrintableTime() const
    {  return printableTime; }

    enum Phase GetPhase() const
    { return phase; }

    void SetPhase(enum Phase value) 
    { phase = value; }

    static enum Phase String2Phase(const string& s)
    {
	if (s == "run") return run;
	return prepare;
    }

    
protected:

    string id;
    int time;
    string printableTime;
    string printableDate;
    enum Phase phase;

};
