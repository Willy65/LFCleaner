# LFCleaner
Utility to strip stray linefeeds from CSV records

We pass a lot of CSV data in the format of:

pattern;<data_1>;<data_2>;<data_3>;<comment>;LF (linefeed)

pattern;<data_1>;<data_2>;<data_4>;<comment>;LF (linefeed)

pattern;<data_1>;<data_3>;<data_5>;<comment>;LF (linefeed)


Every now and then, a user puts in an extra LF (linefeed) in the <comment> field like:
  
pattern;<data_1>;<data_2>;<data_3>;<comment>;LF (linefeed)

pattern;<data_1>;<data_2>;<data_4>;<comLF (bogus linefeed)

ment>;LF (linefeed)

pattern;<data_1>;<data_3>;<data_5>;<comment>;LF (linefeed)
  
  

This breaks the receiving end. It doesn't know how to deal with <com(LF)ment> instead of <comment>.
This C utility can strip those bogus linefeeds, as long as every CSV record has a distinct heading.
