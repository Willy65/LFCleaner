# LFCleaner
Utility to strip bogus linefeeds from CSV records

We pass a lot of CSV data in the format of:

pattern;<data_1>;<data_2>;<data_3>;<-comment-1>;LF (linefeed)

pattern;<data_1>;<data_2>;<data_4>;<-comment-2>;LF (linefeed)

pattern;<data_1>;<data_3>;<data_5>;<-comment-3>;LF (linefeed)


--
Every now and then, a user puts in an extra LF (linefeed) in the <comment> field like:
  
pattern;<data_1>;<data_2>;<data_3>;<-comment-1>;LF (linefeed)

pattern;<data_1>;<data_2>;<data_4>;<-comLF (bogus linefeed)

ment-2>;LF (linefeed)

pattern;<data_1>;<data_3>;<data_5>;<-comment-3>;LF (linefeed)
  
  
--
This breaks the receiving end. It doesn't know how to deal with <-com(LF)ment-2> instead of <-comment-2>.
This C utility can strip those bogus linefeeds, as long as every CSV record has a distinct heading.
It will return with EXIT_FAILURE if a bogus linefeed is found.
The output is the repaired input.
