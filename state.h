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

    string GetProfile() const 
    { return profile; }

    void SetProfile(const string& value)
    { profile = value; }

    string GetPrintableDate() const
    {  return printableDate; }

    string GetPrintableTime() const
    {  return printableTime; }

    enum Phase GetPhase() const
    { return phase; }

    void SetPhase(enum Phase value) 
    { phase = value; }

    string GetStringPhase() const
    { return stringPhase; }

    void SetStringPhase(const string& value) 
    { stringPhase = value; }


    static enum Phase String2Phase(const string& s)
    {
	if (s == "run") return run;
	return prepare;
    }

    double GetTc() const 
    { return Tc; }

    void SetTc(const string& value)
    { SetDoubleFromStr(value, &Tc); }

    double GetTd() const 
    { return Td; }

    void SetTd(const string& value)
    { SetDoubleFromStr(value, &Td); }

    double GetTf() const 
    { return Tf; }

    void SetTf(const string& value)
    { SetDoubleFromStr(value, &Tf); }

    double GetTb() const 
    { return Tb; }

    void SetTb(const string& value)
    { SetDoubleFromStr(value, &Tb); }

    double GetTw() const 
    { return Tw; }

    void SetTw(const string& value)
    { SetDoubleFromStr(value, &Tw); }

    double GetTo() const 
    { return To; }

    void SetTo(const string& value)
    { SetDoubleFromStr(value, &To); }

    double GetTp() const 
    { return Tp; }

    void SetTp(const string& value)
    { SetDoubleFromStr(value, &Tp); }

    string GetFr() const 
    { return fr; }

    void SetFr(const string& value)
    { fr = value; }
    
    string GetHr() const 
    { return hr; }

    void SetHr(const string& value)
    { hr = value; }

    double GetFt() const 
    { return ft; }

    void SetFt(const string& value)
    { SetDoubleFromStr(value, &ft); }
    
    double GetHt() const 
    { return ht; }

    void SetHt(const string& value)
    { SetDoubleFromStr(value, &ht); }

protected:

    string id;
    string profile;
    int time;
    string printableTime;
    string printableDate;
    enum Phase phase;
    string stringPhase;
    double Tc, Td, Tf, Tb, Tw, To, Tp;
    string fr, hr;
    double ft, ht;

    void SetDoubleFromStr(const string& value, double* out)
    {
	if (value == "") 
	{
	    *out = NAN;
	}
	else
	{
	    *out = atof(value.c_str()); 
	}
    }

};
