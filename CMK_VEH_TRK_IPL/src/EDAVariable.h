#pragma once


class CEDAVarRange
{
public:
	CEDAVarRange(void) {}
	virtual ~CEDAVarRange(void) {}
};


class CEDAVariable
{
public:
	CEDAVariable(void) {}
	virtual ~CEDAVariable(void) {}

	virtual bool initVariables(CEDAVarRange* var_range) { return true; }
};