#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstring>
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
   const char infile[] = "/mnt/data/dat/direct/great.dat";
   float t1, t2;
   char recbuf[RECSIZ], keybuf[RECSIZ], fldbuf[FLDSIZ];

   std::cout << "Content-Type: text/html;charset=utf-8\n\n";

   t1 = float(clock()) / CLOCKS_PER_SEC;
   
   cgicc::Cgicc cgi;
   cgicc::form_iterator search = cgi.getElement("search");
   cgicc::form_iterator pt = cgi.getElement("key");
   cgicc::form_iterator select = cgi.getElement("select");
   cgicc::form_iterator order = cgi.getElement("order");
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
   std::ifstream ifstr(infile, std::ifstream::in | std::ifstream::binary);
   memset(recbuf, '\0', RECSIZ);
   while ( ifstr.read(recbuf, RECSIZ) ) {
      memset(keybuf, '\0', RECSIZ);
      for (int i = 0; i < keyfields.size(); i++) {
         if ( i > 0 ) strcat(keybuf, ",");
         getField(fldbuf, recbuf, keyfields[i]);
         strcat(keybuf, fldbuf);
      }
      
      if ( std::string(keybuf) == target ) {
         for (int i = 0; i < ordfields.size(); i++) {
            // strcat(keybuf, ",");
            getField(fldbuf, recbuf, ordfields[i]);
            strcat(keybuf, fldbuf);
         }
         Tree->insert( std::string(keybuf) , n);   
      }
      
      memset(recbuf, '\0', RECSIZ);
      ++n;
   }
   ifstr.close();

   // std::vector<int> idx = Tree->search(target);
   std::vector<int> idx;
   Tree->loadIdxVector(idx);
   delete Tree;

   printHeader();

   if ( idx.size() > 0 ) {
      std::cout << "<table id=\"my_table\">\n";
      printHeader(selflds);
      ifstr.open(infile, std::ifstream::in | std::ifstream::binary);
      for (int i = 0; i < idx.size(); i++) {
         memset(recbuf, '\0', RECSIZ);
         ifstr.seekg(idx[i] * RECSIZ);
         ifstr.read(recbuf, RECSIZ);
         std::cout << "<tr>";
         for (int i = 0; i < selflds.size(); i++) {
            getField(fldbuf, recbuf, selflds[i]);
            std::cout << "<td>" << fldbuf << "</td>";
         }
         std::cout << "</tr>\n";
      }
      ifstr.close();
      std::cout << "</table>\n";
   } else {
      std::cout << "<p>no results found</p>\n";
   }
   
   t2 = float(clock()) / CLOCKS_PER_SEC;

   std::cout << "<ul>\n";
   std::cout.precision(3);
   std::cout << std::fixed;
   std::cout << "<li>elapsed time = " << t2-t1 << " seconds</li>\n";
   std::cout << "<li>no. of rows in relation = " << idx.size() << "</li>\n";
   std::cout << "</ul>\n";

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
   std::cout << "<div><a href='/index.html'>Home</a> | <a href='/avlbase.html'>Back</a></div>\n";
   std::cout << "<br>\n";
}

void printFooter()
{
   std::cout << "<br>\n";
   std::cout << "<div><a href='/index.html'>Home</a> | <a href='/avlbase.html'>Back</a></div>\n";
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
