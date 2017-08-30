import sys
import urllib
from HTMLParser import HTMLParser

def main():
   if( len( sys.argv) != 2 ):
      quit()
   else:
      getthething( sys.argv[1] )

def getthething( url ):
   f = urllib.urlopen(url)

   class MyHTMLParser(HTMLParser):
      checkforbox = False
      checkforurl = False
      setsend = False
      sendurl = ""

      def handle_starttag(self, tag, attrs):
         if( tag == "table"):
            for i in attrs:
               if( i[0] == "class" and i[1] == "problems" ):
                  self.checkforbox = True
         elif( tag ==  "tr" and self.checkforbox == True ):
            #self.checkforbox = False
            self.checkforurl = True
         elif( tag ==  "a" and self.checkforurl == True ):
            self.checkforurl = False
            self.setsend = True
            for i in attrs:
               if( i[0] == "href"):
                  self.sendurl = i[1]


      def handle_endtag(self, tag):
         if( tag == "table"):
            self.checkforbox = False
            self.checkforurl = False

      def handle_data(self, data):
         if( self.setsend == True ):
            dothething( data.strip(), "http://codeforces.com" + self.sendurl )
            self.setsend = False


   parser = MyHTMLParser()
   parser.feed( f.read() )

def dothething( letter, url ):

   f = urllib.urlopen(url)

   class MyHTMLParser(HTMLParser):
      num = 0
      myfile = None
      newin = False
      newdata = False
      preblock = False
      def handle_starttag(self, tag, attrs):
         if( tag == "div"):
            for i in attrs:
               if( i[0] == "class" and i[1] == "input" ):
                  self.newin = True
         pass

         if( tag == "pre" and self.newin == True ):
            self.newin = False
            self.newdata = True
            self.preblock = True


      def handle_endtag(self, tag):
         if( tag == "pre" and self.preblock == True ):
            self.preblock = False
            self.num = self.num + 1
            self.myfile.close()

      def handle_data(self, data):
         if( self.newdata == True ):
            self.myfile = open( letter + str(self.num) + ".in" , 'w' );
            self.myfile.write( data + '\n' )
            self.newdata = False

         elif( self.preblock == True ):
            self.myfile.write(  data + '\n')

   parser = MyHTMLParser()
   parser.feed( f.read() )

if __name__ == "__main__":
   main()
