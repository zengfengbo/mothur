/*
 *  classify.cpp
 *  Mothur
 *
 *  Created by westcott on 11/3/09.
 *  Copyright 2009 Schloss Lab. All rights reserved.
 *
 */

#include "classify.h"
#include "sequence.hpp"
#include "kmerdb.hpp"
#include "suffixdb.hpp"
#include "blastdb.hpp"
#include "distancedb.hpp"

/**************************************************************************************************/
void Classify::generateDatabaseAndNames(string tfile, string tempFile, string method, int kmerSize, float gapOpen, float gapExtend, float match, float misMatch)  {		
	try {
        maxLevel = 0;
		taxFile = tfile;
		
		int numSeqs = 0;

        templateFile = tempFile;
        
        int start = time(NULL);
        
        m->mothurOut("Generating search database...    "); cout.flush();
        
        //need to know number of template seqs for suffixdb
        if (method == "suffix") {
            ifstream inFASTA;
            m->openInputFile(tempFile, inFASTA);
            m->getNumSeqs(inFASTA, numSeqs);
            inFASTA.close();
        }
        
        bool needToGenerate = true;
        string kmerDBName;
        if(method == "kmer")			{
            database = new KmerDB(tempFile, kmerSize);
            
            kmerDBName = tempFile.substr(0,tempFile.find_last_of(".")+1) + char('0'+ kmerSize) + "mer";
            ifstream kmerFileTest(kmerDBName.c_str());
            if(kmerFileTest){
                bool GoodFile = m->checkReleaseVersion(kmerFileTest, m->getVersion());
                int shortcutTimeStamp = m->getTimeStamp(kmerDBName);
                int referenceTimeStamp = m->getTimeStamp(tempFile);
                
                //if the shortcut file is older then the reference file, remake shortcut file
                if (shortcutTimeStamp < referenceTimeStamp) {  GoodFile = false;  }

                if (GoodFile) {  needToGenerate = false;	}
            }
        }
        else if(method == "suffix")		{	database = new SuffixDB(numSeqs);								}
        else if(method == "blast")		{	database = new BlastDB(tempFile.substr(0,tempFile.find_last_of(".")+1), gapOpen, gapExtend, match, misMatch, "", threadID);	}
        else if(method == "distance")	{	database = new DistanceDB();	}
        else {
            m->mothurOut(method + " is not a valid search option. I will run the command using kmer, ksize=8.");
            m->mothurOutEndLine();
            database = new KmerDB(tempFile, 8);
        }
        
        if (needToGenerate) {
            ifstream fastaFile;
            m->openInputFile(tempFile, fastaFile);
            
            while (!fastaFile.eof()) {
                Sequence temp(fastaFile);
                m->gobble(fastaFile);
                
                names.push_back(temp.getName());
                
                database->addSequence(temp);
            }
            fastaFile.close();
            
            if ((method == "kmer") && (!shortcuts)) {;} //don't print
            else {database->generateDB(); }
            
        }else if ((method == "kmer") && (!needToGenerate)) {
            ifstream kmerFileTest(kmerDBName.c_str());
            database->readKmerDB(kmerFileTest);
            
            ifstream fastaFile;
            m->openInputFile(tempFile, fastaFile);
            
            while (!fastaFile.eof()) {
                Sequence temp(fastaFile);
                m->gobble(fastaFile);
                
                names.push_back(temp.getName());
            }
            fastaFile.close();
        }
        
        database->setNumSeqs(names.size());
        
        m->mothurOut("DONE."); m->mothurOutEndLine();
        m->mothurOut("It took " + toString(time(NULL) - start) + " seconds generate search database. "); m->mothurOutEndLine();
        
        readTaxonomy(taxFile);
        
        //sanity check
        bool okay = phyloTree->ErrorCheck(names);
        
        if (!okay) { m->control_pressed = true; }
	}
	catch(exception& e) {
		m->errorOut(e, "Classify", "generateDatabaseAndNames");
		exit(1);
	}
}
/**************************************************************************************************/
Classify::Classify() {		m = MothurOut::getInstance();	database = NULL;	phyloTree=NULL; flipped=false;  maxLevel = 0; }
/**************************************************************************************************/

int Classify::readTaxonomy(string file) {
	try {
		
		phyloTree = new PhyloTree();
		string name, taxInfo;
		
		m->mothurOut("\nReading in the " + file + " taxonomy...\t");	cout.flush();
        if (m->debug) { m->mothurOut("[DEBUG]: Taxonomies read in...\n"); }
        
        taxonomy.clear(); 
        m->readTax(file, taxonomy, true);
        
        for (map<string, string>::iterator itTax = taxonomy.begin(); itTax != taxonomy.end(); itTax++) {
            phyloTree->addSeqToTree(itTax->first, itTax->second);
            if (m->control_pressed) { break; }
        }

		phyloTree->assignHeirarchyIDs(0);
        
        maxLevel = phyloTree->getMaxLevel();
		
		phyloTree->setUp(file);

		m->mothurOut("DONE.");
		m->mothurOutEndLine();	cout.flush();
		
		return phyloTree->getNumSeqs();
	
	}
	catch(exception& e) {
		m->errorOut(e, "Classify", "readTaxonomy");
		exit(1);
	}
}
/**************************************************************************************************/

vector<string> Classify::parseTax(string tax) {
	try {
		vector<string> taxons;
		m->splitAtChar(tax, taxons, ';');
        return taxons;
	}
	catch(exception& e) {
		m->errorOut(e, "Classify", "parseTax");
		exit(1);
	}
}
/**************************************************************************************************/

double Classify::getLogExpSum(vector<double> probabilities, int& maxIndex){
	try {
        //	http://jblevins.org/notes/log-sum-exp
        
        double maxProb = probabilities[0];
        maxIndex = 0;
        
        int numProbs = (int)probabilities.size();
        
        for(int i=1;i<numProbs;i++){
            if(probabilities[i] >= maxProb){
                maxProb = probabilities[i];
                maxIndex = i;
            }
        }
        
        double probSum = 0.0000;
        
        for(int i=0;i<numProbs;i++){
            probSum += exp(probabilities[i] - maxProb);		
        }
        
        probSum = log(probSum) + maxProb;
        
        return probSum;
	}
	catch(exception& e) {
		m->errorOut(e, "Classify", "getLogExpSum");
		exit(1);
	}
}

/**************************************************************************************************/

