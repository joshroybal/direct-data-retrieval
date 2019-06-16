#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cgicc/Cgicc.h>
#include "csv.hpp"
#include "avl.hpp"

enum { dummy, first, last, company, address, city, county, state, zip, phone, 
   cell, email, url };

void error();
void printHeader(std::vector<int>&);
void printHeader();
void printFooter();
void printCopyright();

int main()
{
   const int RECSIZ = 512;
   const char infile[] = "/mnt/data/medium.dat";
   char buf[RECSIZ];

   std::cout << "Content-Type: text/html;charset=utf-8\n\n";
   
   cgicc::Cgicc cgi;
   cgicc::form_iterator search = cgi.getElement("search");
   cgicc::form_iterator pt = cgi.getElement("key");
   cgicc::form_iterator select = cgi.getElement("select");
   cgicc::form_iterator order = cgi.getElement("order");
   // std::cout << "key = " << **pt << std::endl;
   std::string target(**pt);
   if ( target.length() == 0 ) {
      error();
      return 1;
   }


   // pick up search fields - trap some foreseen errors
   std::istringstream isstr(**search);
   std::vector<int> keyfields;
   int tmp;   
   while ( isstr >> tmp ) {
      if ( tmp < 1 || tmp > 12 ) {
         error();
         return 1;
      } 
      keyfields.push_back(tmp);
   }
   if ( keyfields.size() == 0 ) {
      error();
      return 1;
   }
   // pick up selection fields
   std::istringstream iselstr(**select);
   std::vector<int> selflds;
   while ( iselstr >> tmp ) {
      if ( tmp < 1 || tmp > 12 ) {
         error();
         return 1;
      } 
      selflds.push_back(tmp);
   }
   if ( selflds.size() == 0 ) {
      error();
      return 1;
   }   
   // pick up fields to order by
   std::istringstream ordsstr(**order);
   std::vector<int> ordfields;
   while ( ordsstr >> tmp ) ordfields.push_back(tmp);

   int n = 0;
   AVL* Tree = new AVL();
   std::string record;
   std::ifstream ifstr(infile, std::ifstream::in | std::ifstream::binary);
   while ( ifstr.read(buf, RECSIZ) ) {
      std::string record(buf);
      std::ostringstream osstr;
      for (int i = 0; i < keyfields.size(); i++) {
         osstr << getField(record, keyfields[i]);
         if (i < keyfields.size() - 1) osstr << ',';
      }
      Tree->insert(osstr.str(), n++);   
   }
   ifstr.close();
   std::vector<int> idx = Tree->search(target);
   delete Tree;

   printHeader();

   if ( idx.size() > 0 ) {
      if ( ordfields.size() > 0 ) { // re-order results if necessary
         Tree = new AVL(); 
         ifstr.open(infile, std::ifstream::in | std::ifstream::binary);
         for (int i = 0; i < idx.size(); i++) {
            ifstr.seekg(idx[i] * RECSIZ);
            ifstr.read(buf, RECSIZ);
            std::string record(buf);
            std::ostringstream osstr;
            for (int j = 0; j < ordfields.size(); j++) {
               osstr << getField(record, ordfields[j]);
               if (j < ordfields.size() - 1) osstr << ',';
            }
            Tree->insert(osstr.str(), idx[i]);
         }
         ifstr.close();
         idx.clear();
         Tree->loadIdxVector(idx);
         delete Tree;
      }

      std::cout << "<table id=\"my_table\">\n";
      printHeader(selflds);
      ifstr.open(infile, std::ifstream::in | std::ifstream::binary);
      for (int i = 0; i < idx.size(); i++) {
         ifstr.seekg(idx[i] * RECSIZ);
         ifstr.read(buf, RECSIZ);
         std::string record(buf);
         std::cout << "<tr>";
         for (int i = 0; i < selflds.size(); i++) {
            std::cout << "<td>" << getField(record, selflds[i]) << "</td>";
         }
         std::cout << "</tr>\n";
      }
      ifstr.close();
      std::cout << "</table>\n";
   } else {
      std::cout << "<p>no results found</p>\n";
   }
   
   printFooter();

   return 0;   
}

void error()
{
   printHeader();
   std::cout << "<p>I'm sorry Dave, I'm afraid I can't do that.</p>\n";
   printFooter();
}

void printHeader(std::vector<int>& fldidx)
{
   std::cout << "<tr>";
   for (int i = 0; i < fldidx.size(); i++) {
      if ( fldidx[i] == first  ) std::cout << "<th>fist name</th>";
      if ( fldidx[i] == last ) std::cout << "<th>last name</th>";
      if ( fldidx[i] == company ) std::cout << "<th>company</th>";
      if ( fldidx[i] == address ) std::cout << "<th>address</th>";
      if ( fldidx[i] == city ) std::cout << "<th>city</th>";
      if ( fldidx[i] == county ) std::cout << "<th>county</th>";
      if ( fldidx[i] == state ) std::cout << "<th>state</th>";
      if ( fldidx[i] == zip ) std::cout << "<th>zip code</th>";
      if ( fldidx[i] == phone  ) std::cout << "<th>phone no.</th>";
      if ( fldidx[i] == cell ) std::cout << "<th>cell no.</th>";
      if ( fldidx[i] == email ) std::cout << "<th>e-mail</th>";
      if ( fldidx[i] == url ) std::cout << "<th>web page</th>";
   }
   std::cout << "</tr>\n";
}

void printHeader()
{
   std::cout << "<!DOCTYPE html>\n";
   std::cout << "<html>\n";
   std::cout << "<head>\n";
   std::cout << "<link id='styleinfo' media='all'>\n";
   std::cout << "<script type='text/javascript' src='/avlbase.js' defer></script>\n";
   std::cout << "</head>\n";
   std::cout << "<body>\n";
   std::cout << "<header><p>C++ AVL balanced binary search tree data retrieval</p></header>\n";
}

void printFooter()
{
   std::cout << "<footer><p>";
   printCopyright();
   std::cout << "</p></footer>\n";
   std::cout << "</body>\n";
   std::cout << "</html>\n";
}

void printCopyright()
{
   std::cout << "GNU cgicc - A C++ class library for writing CGI applications";
   std::cout << "<br>";
   std::cout << "Copyright &#xa9; 1996 - 2004 Stephen F. Booth";
   std::cout << "<br>";
   std::cout << "Otherwise Copyright &#xa9; 2019 Joshua E. Roybal";
}
