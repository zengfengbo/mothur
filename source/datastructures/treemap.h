#ifndef TREEMAP_H
#define TREEMAP_H
/*
 *  treemap.h
 *  Mothur
 *
 *  Created by Sarah Westcott on 1/26/09.
 *  Copyright 2009 Schloss Lab UMASS Amherst. All rights reserved.
 *
 */

#include "mothur.h"
#include "listvector.hpp"

/* This class is used by the read.tree command to build the tree container. */

struct GroupIndex {
	string	groupname;
	int		vectorIndex;
};

class TreeMap {
public:
	TreeMap() { m = MothurOut::getInstance(); }
	TreeMap(string);
	~TreeMap();
    
	int readMap();
    int readMap(string);
	int getNumGroups();
	int getNumSeqs();
	//void setIndex(string, int);  //sequencename, index
	//int getIndex(string);		//returns vector index of sequence
	bool isValidGroup(string);  //return true if string is a valid group
	void removeSeq(string);  //removes a sequence, this is to accomadate trees that do not contain all the seqs in your groupfile
	string getGroup(string);
	void addSeq(string, string);
	void addGroup(string s) { setNamesOfGroups(s); }
	vector<string> getNamesOfGroups() {
		sort(namesOfGroups.begin(), namesOfGroups.end());
		return namesOfGroups;
	}
    
    void print(ostream&);
	void makeSim(vector<string>);  //takes groupmap info and fills treemap for use by tree.shared command.
	void makeSim(ListVector*);  //takes listvector info and fills treemap for use by tree.shared command.	
    vector<string> getNamesSeqs();
	vector<string> getNamesSeqs(vector<string>); //get names of seqs belonging to a group or set of groups
    int getCopy(TreeMap&);
    
    vector<string> namesOfSeqs;
    map<string,int> seqsPerGroup;	//groupname, number of seqs in that group.
	map<string, GroupIndex> treemap; //sequence name and <groupname, vector index>

    
private:
	vector<string> namesOfGroups;
	ifstream fileHandle;
	string groupFileName;
	int numGroups;
	map<string, GroupIndex>::iterator it;
	map<string, int>::iterator it2;
	void setNamesOfGroups(string); 
	MothurOut* m;
	
	
};

#endif
