package moca.logging;

import java.util.logging.Formatter;
import java.util.logging.LogRecord;
import java.util.Calendar;
import java.util.GregorianCalendar;

public class DefaultFormatter extends Formatter {

	public String format(LogRecord record) {
		StringBuilder string = new StringBuilder();
		string.append("[");
		string.append(formatLoggerName(record.getLoggerName()));
		string.append("] [");
		string.append(record.getLevel());
		string.append("] - <");
		string.append(formatDate(record.getMillis()));
		string.append(">\n\t");
		string.append(record.getMessage());
		string.append('\n');
		return string.toString();
	}


	/* Logger name */
	public String formatLoggerName(String loggerName) {
		String[] sub = loggerName.split(".");
		if (sub.length > 1) {
			if (sub[0].compareTo(Logger.DEFAULT_LOGGER_NAME) == 0)
				return sub[sub.length-1];
		}
		return loggerName;
	}


	/* Date */

	public static final String DATE_SEPARATOR = "/";
	public static final String DATE_TIME_SEPARATOR = " ";
	public static final String TIME_SEPARATOR = ":";

	public String formatDate(long milliseconds) {
		_calendar.setTimeInMillis(milliseconds);
		StringBuilder dateString = new StringBuilder();
		dateString.append(_calendar.get(Calendar.YEAR));
		dateString.append(DATE_SEPARATOR);
		dateString.append(_calendar.get(Calendar.MONTH));
		dateString.append(DATE_SEPARATOR);
		// day of month starts at 0
		dateString.append(_calendar.get(Calendar.DAY_OF_MONTH) + 1);
		dateString.append(DATE_TIME_SEPARATOR);
		dateString.append(_calendar.get(Calendar.HOUR_OF_DAY));
		dateString.append(TIME_SEPARATOR);
		dateString.append(_calendar.get(Calendar.MINUTE));
		dateString.append(TIME_SEPARATOR);
		dateString.append(_calendar.get(Calendar.SECOND));
		return dateString.toString();
	}

	private Calendar _calendar = null;

	/* Singleton */
	public static DefaultFormatter instance() {
		if (__instance == null)
			__instance = new DefaultFormatter();
		return __instance;
	}
	private static DefaultFormatter __instance = null;

	protected DefaultFormatter() {
		_calendar = new GregorianCalendar();
	}

};

