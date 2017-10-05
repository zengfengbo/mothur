#ifndef SCREENSEQSCOMMAND_H
#define SCREENSEQSCOMMAND_H

/*
 *  screenseqscommand.h
 *  Mothur
 *
 *  Created by Pat Schloss on 6/3/09.
 *  Copyright 2009 Patrick D. Schloss. All rights reserved.
 *
 */
#include "mothur.h"
#include "command.hpp"
#include "sequence.hpp"

class ScreenSeqsCommand : public Command {
	
public:
	ScreenSeqsCommand(string);
	ScreenSeqsCommand();
	~ScreenSeqsCommand() {}
	
	vector<string> setParameters();
	string getCommandName()			{ return "screen.seqs";				}
	string getCommandCategory()		{ return "Sequence Processing";		}
	
	string getHelpString();	
    string getOutputPattern(string);	
	string getCitation() { return "http://www.mothur.org/wiki/Screen.seqs"; }
	string getDescription()		{ return "enables you to keep sequences that fulfill certain user defined criteria"; }

	int execute(); 
	void help() { m->mothurOut(getHelpString()); }	
	
	
private:
    int optimizeContigs();
    int optimizeAlign();
	int createProcesses(string, string, string, map<string, string>&);
    int screenSummary(map<string, string>&);
    int screenContigs(map<string, string>&);
    int screenAlignReport(map<string, string>&);
    int runFastaScreening(map<string, string>&);
    int screenFasta(map<string, string>&);
    int screenReports(map<string, string>&);
	int getSummary();
    int getSummaryReport();
 
    bool abort;
    string fastafile, namefile, groupfile, alignreport, outputDir, qualfile, taxonomy, countfile, contigsreport, summaryfile, fileType, badAccnosFile;
	int startPos, endPos, maxAmbig, maxHomoP, minLength, maxLength, processors, minOverlap, oStart, oEnd, mismatches, maxN, maxInsert;
    float minSim, minScore, criteria;
	vector<string> outputNames;
	vector<string> optimize;
	map<string, int> nameMap;
	
    
};
/**************************************************************************************************/
//custom data structure for threads to use.
// This is passed by void pointer so it can be any data type
// that can be passed using a single void pointer (LPVOID).
struct sumScreenData {
    int startPos, endPos, maxAmbig, maxHomoP, minLength, maxLength, maxN;
	unsigned long long start;
	unsigned long long end;
	int count;
	MothurOut* m;
	string goodFName, badAccnosFName, filename;
    map<string, string> badSeqNames;
    string summaryfile, contigsreport;
	
	
	sumScreenData(){}
	sumScreenData(int s, int e, int a, int h, int minl, int maxl, int mn, map<string, string> bs, string f, string sum, string cont, MothurOut* mout, unsigned long long st, unsigned long long en, string gf, string bf) {
		startPos = s;
		endPos = e;
		minLength = minl;
        maxLength = maxl;
		maxAmbig = a;
		maxHomoP = h;
        maxN = mn;
		filename = f;
        goodFName = gf;
        badAccnosFName = bf;
		m = mout;
		start = st;
		end = en;
        summaryfile = sum;
        contigsreport = cont;
        badSeqNames = bs;
		count = 0;
	}
};
/**************************************************************************************************/

#endif
