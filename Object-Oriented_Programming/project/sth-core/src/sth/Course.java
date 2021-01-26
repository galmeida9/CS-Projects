package sth;

import java.io.Serializable;
import java.util.Map;
import java.util.TreeMap;
import java.util.List;
import java.util.LinkedList;

class Course implements Serializable{

	private String _name;
	private Map<String, Discipline> _disciplines = new TreeMap<String, Discipline>();
	private Map<Integer, Student> _representatives = new TreeMap<Integer, Student>();
	private Map<Integer, Student> _students = new TreeMap<Integer, Student>();

	public Course(String name){
		_name = name;
	}
	
	public String getName(){ return _name; }
	
	public void addStudent(Student s){
		_students.put(s.getId(), s);
	}

	public void addRepresentative(Student r){
		if (_representatives.size()<=7)
			_representatives.put(r.getId(), r);
	}

	public void removeRepresentative(Student r){
		_representatives.remove(r.getId());
	}
	
	public List<Watcher> getWatchers(){
		List<Watcher> res = new LinkedList<Watcher>();
		res.addAll(_representatives.values());
		return res;
	}
	
	public void addDiscipline(Discipline d){
		_disciplines.put(d.getName(), d);
	}
	
	public boolean containsDiscipline(String name){
		return _disciplines.containsKey(name);
	}

	public Discipline getDiscipline(String name){
		return _disciplines.get(name);
	}

	public boolean equals(Object o){
		if (o instanceof Course){
			Course course = (Course) o;
			return _name.equals(course._name);
		}
		return false;
	}

	public String toString(){
		return _name;
	}

	public boolean isRepFull() {
		if (_representatives.size() < 7) return false;
		return true;
	}
}

