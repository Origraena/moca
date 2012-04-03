package moca.logging;

import java.util.HashMap;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Handler;
import java.util.logging.ConsoleHandler;
import java.util.logging.LogManager;

/**
 * Logger convenient class.<br />
 * Maintains the logger instances to not be garbage collected.<br /> 
 * Provides some functions usefull to get quickly a specific instance for an object or a
 * class.
 * Most of the methods has a corresponding static method with same name but the first
 * character.<br />
 * Further more, this class link every handler added by the addHandler method to each
 * loggerinlogger instance.<br />
 * By default, a console handler is added using the default formatter class.<br />
 * Instances are managed by an internal hash map providing average O(1) access to the
 * loggers.
 */
public class Logger {

	/** Default logger name */
	public static final String DEFAULT_LOGGER_NAME = "moca";


	/* Handlers methods */
	public boolean addHandler(Handler h) {
		if (_handlers.contains(h))
			return false;
		_handlers.add(h);
		for (java.util.logging.Logger l : _loggers.values())
			l.addHandler(h);
		return true;
	}


	/* GET methods */
	public java.util.logging.Logger getLogger(String name) {
		java.util.logging.Logger result = _loggers.get(name);
		if (result == null) {
			result = java.util.logging.Logger.getLogger(name);
			_loggers.put(name,result);
			for (Handler h : _handlers)
				result.addHandler(h);
		}
		return result;
	}
	public java.util.logging.Logger getClassLogger(Class<?> c) {
		return getLogger(c.getName());
	}
	public java.util.logging.Logger getClassLogger(Object o) {
		return getLogger(o.getClass().getName());
	}
	public java.util.logging.Logger getLogger(Object o) {
		return getLogger(o.getClass().getName() + '@' + Integer.toHexString(o.hashCode()));
	}
	public static java.util.logging.Logger sgetLogger(String name) {
		return instance().getLogger(name);
	}
	public static java.util.logging.Logger sgetClassLogger(Class<?> c) {
		return instance().getClassLogger(c);
	}
	public static java.util.logging.Logger sgetClassLogger(Object o) {
		return instance().getClassLogger(o);
	}
	public static java.util.logging.Logger sgetLogger(Object o) {
		return instance().getLogger(o);
	}


	/* Log methods */
	public void log(String loggerName, Level level, String msg) {
		getLogger(loggerName).log(level,msg);
	}
	public void log(Level level, String msg) {
		log(DEFAULT_LOGGER_NAME,level,msg);
	}
	public void log(Object o, Level level, String msg) {
		getLogger(o).log(level,msg);
	}
	public static void slog(String loggerName, Level level, String msg) {
		instance().log(loggerName,level,msg);
	}
	public static void slog(Level level, String msg) {
		instance().log(level,msg);
	}
	public static void slog(Object o, Level level, String msg) {
		instance().log(o,level,msg);
	}

	/* Info methods */
	public void info(String loggerName, String msg) {
		log(loggerName,Level.INFO,msg);
	}
	public void info(String msg) {
		log(DEFAULT_LOGGER_NAME,Level.INFO,msg);
	}
	public void info(Object o, String msg) {
		getLogger(o).info(msg);
	}
	public void sinfo(String loggerName, String msg) {
		instance().info(loggerName,msg);
	}
	public void sinfo(String msg) {
		instance().info(msg);
	}
	public void sinfo(Object o, String msg) {
		instance().info(o,msg);
	}

	/* Warning methods */
	public void warning(String loggerName, String msg) {
		log(loggerName,Level.WARNING,msg);
	}
	public void warning(String msg) {
		log(DEFAULT_LOGGER_NAME,Level.WARNING,msg);
	}
	public void warning(Object o, String msg) {
		getLogger(o).warning(msg);
	}
	public static void swarning(String loggerName, String msg) {
		instance().warning(loggerName,msg);
	}
	public static void swarning(String msg) {
		instance().warning(msg);
	}
	public static void swarning(Object o, String msg) {
		instance().warning(o,msg);
	}

	/* Severe methods */
	public void severe(String loggerName, String msg) {
		log(loggerName,Level.SEVERE,msg);
	}
	public void severe(String msg) {
		log(DEFAULT_LOGGER_NAME,Level.SEVERE,msg);
	}
	public void severe(Object o, String msg)  {
		getLogger(o).severe(msg);
	}
	public static void ssevere(String loggerName, String msg) {
		instance().severe(loggerName,msg);
	}
	public static void ssevere(String msg) {
		instance().severe(msg);
	}
	public static void ssevere(Object o, String msg)  {
		instance().severe(o,msg);
	}


	/* Members */
	private HashMap<String,java.util.logging.Logger> _loggers;
	private ArrayList<Handler> _handlers;

	/* SINGLETON */
	public static Logger instance() {
		if (__instance == null)
			__instance = new Logger();
		return __instance;
	}
	private static Logger __instance = null;
	private Logger() { 
		LogManager.getLogManager().reset();	// remove the default console handler
		_loggers = new HashMap<String,java.util.logging.Logger>();
		_handlers = new ArrayList<Handler>();
		ConsoleHandler h = new ConsoleHandler();
		h.setFormatter(DefaultFormatter.instance());
		addHandler(h);
	}

};

