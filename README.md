# LFCleaner
Utility to strip bogus linefeeds from CSV records and outputs repaired data<br>
We pass a lot of CSV data in the format of:<br>

<pre>
pattern;[data_1];[data_2];[data_4];[comment-1];LF (linefeed)
pattern;[data_1];[data_2];[data_5];[comment-2];LF (linefeed)
pattern;[data_1];[data_3];[data_6];[comment-3];LF (linefeed)
</pre>

Every now and then, a user puts in an extra LF (linefeed) in the [comment] field like:<br>
<pre>
pattern;[data_1];[data_2];[data_4];[comment-1];LF (linefeed)
pattern;[data_1];[data_2];[data_5];[comLF<br>ment-2];LF (linefeed)
pattern;[data_1];[data_3];[data_6];[comment-3];LF (linefeed)
</pre>

This breaks the receiving end.<br>
It doesn't know how to deal with [com(LF)ment-2] instead of [comment-2].<br>
This C utility can strip those bogus linefeeds, as long as every CSV record has a distinct heading.<br>
Exitcode = EXIT_FAILURE if a bogus linefeed is found.<br>

The output is the repaired input.
