package sth;

import java.io.Serializable;
import java.util.Comparator;
import java.util.List;
import java.util.LinkedList;
import sth.exceptions.OperationUnsupportedException;

abstract class Person implements Comparable<Person>, Watcher, Serializable{
	private String _name;
	private String _telephone;
	private int _id;
	private List<String> _notifications;
	private List<Watchable> _watched;

	public Person(String name, String telephone, int id){
		_name = name; _telephone = telephone; _id = id;
		_notifications = new LinkedList<String>();
		_watched = new LinkedList<Watchable>();
	}

	public String showPerson(){
		return "|"+_id+"|"+_telephone+"|"+_name;
	}

	public void changeTelephone(String telephone){
		_telephone = telephone;
	}

	public String getName(){
		return _name;
	}

	public int getId() {
		return _id;
	}

	public boolean nameContains(String name){
		return _name.contains(name);
	}

	public int compareTo(Person o){
		return _name.compareTo(o.getName());
	}
	
	public abstract void addDiscipline(Discipline d) throws OperationUnsupportedException;
	public abstract String getDisciplines();
	public abstract void addCourse(Course c) throws OperationUnsupportedException;
	public abstract boolean containsDiscipline(String name) throws OperationUnsupportedException;
	public abstract Discipline getDiscipline(String name) throws OperationUnsupportedException;
	
	public void addWatchable(Watchable o){
		if (!_watched.contains(o)) _watched.add(o);
	}

	public void removeWatchable(Watchable o){
		_watched.remove(o);
	}

	public void update(String msg){
		_notifications.add(msg);
	}

	public String[] sendNotifications(){
		String[] res = _notifications.toArray(new String[_notifications.size()]);
		_notifications.clear();
		return res;
	}

	public boolean equals(Object o){
		if (o instanceof Person){
			Person p = (Person) o;
			return _name.equals(p.getName());
		}
		return false;
	}
}

