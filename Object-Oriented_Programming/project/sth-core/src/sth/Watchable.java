package sth;

interface Watchable{
	public void registerWatcher(Watcher o);
	public void removeWatcher(Watcher o);
	public void notifyWatchers(String msg);
}
