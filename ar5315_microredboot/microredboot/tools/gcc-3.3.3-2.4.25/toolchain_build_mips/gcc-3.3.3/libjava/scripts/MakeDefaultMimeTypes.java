/* Copyright (C) 2000  Free Software Foundation

   This file is part of libgcj.

This software is copyrighted work licensed under the terms of the
Libgcj License.  Please consult the file "LIBGCJ_LICENSE" for
details.  */

import gnu.gcj.io.MimeTypes;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.util.Hashtable;
import java.util.Enumeration;
import java.util.NoSuchElementException;

public class MakeDefaultMimeTypes
{
  private static void fatal (String message)
  {
    System.err.println ("MakeDefaultMimeTypes Error: " + message);
    System.exit (-1);
  }
  
  public static void main (String[] args)
  {
    Hashtable mime_table = new Hashtable ();
    
    if (args.length != 1)
      fatal ("missing mime type filename");

    try {
      MimeTypes.fillFromFile (mime_table, args[0]);
    } catch (FileNotFoundException ex) {
      fatal ("can't open " + args[0]);
    } catch (IOException ex) {
      fatal ("error reading " + args[0]);
    }

    System.out.println ("// Do not edit this file!  Create a new version with MakeDefaultMimeTypes.\
\
/* Copyright (C) 2000  Free Software Foundation\
\
   This file is part of libgcj.\
\
This software is copyrighted work licensed under the terms of the\
Libgcj License.  Please consult the file \"LIBGCJ_LICENSE\" for\
details.  */\
\
package gnu.gcj.io; \
\
public class DefaultMimeTypes\
{\
  public static final String[] types = {");

    Enumeration keys = mime_table.keys();
    Enumeration values = mime_table.elements();
    
    // Prepend first element with open bracket
    StringBuffer result = new StringBuffer("");
    
    try
      {
	result.append("      \"" 
		      + keys.nextElement().toString() 
		      + "\",\t\"" 
		      + values.nextElement().toString()
		      + "\"\n");
      }
    catch (NoSuchElementException ex)
      {
      }
    
    // Prepend subsequent elements with ", "
    try
      {
	while (true)
	  result.append("    , \"" 
			+ keys.nextElement().toString() 
			+ "\",\t\"" 
			+ values.nextElement().toString()
			+ "\"\n");
      }
    catch (NoSuchElementException ex)
      {
      }
    
    // Append last element with closing bracket
    result.append("  };\
}\
");
    System.out.println(result);
  }
}
