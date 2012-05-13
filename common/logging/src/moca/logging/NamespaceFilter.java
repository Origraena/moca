package moca.logging;

import java.util.logging.LogRecord;
import java.util.logging.Filter;

/**
 * Filters all logs from classes which are not in the specified namespace.
 * <br />
 * Contains a static instance for moca package.
 * @see String#startsWith(String)
 */
public class NamespaceFilter implements Filter {

	public NamespaceFilter(String namespace) {
		if (namespace == null)
			_namespace = "";
		else
			_namespace = namespace;
	}

	public boolean isLoggable(LogRecord record) {
		return record.getSourceClassName().startsWith(_namespace);
	}

	private String _namespace; 


	/* Specific instances */

	public static Filter defaultFilter() {
		if (__defaultFilter == null)
			__defaultFilter = new NamespaceFilter(Logger.DEFAULT_LOGGER_NAME);
		return __defaultFilter;
	}

	private static Filter __defaultFilter = null;

};

