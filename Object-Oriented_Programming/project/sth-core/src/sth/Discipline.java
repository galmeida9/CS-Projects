package sth;

import java.io.Serializable;
import java.util.List;
import java.util.LinkedList;
import java.util.Map;
import java.util.TreeMap;
import java.util.Set;
import java.util.HashSet;
import java.util.Collections;
import java.util.Collection;
import sth.exceptions.DisciplineFullOnStudentsException;

class Discipline implements Comparable<Discipline>, Watchable, Serializable{

	private	String _name;
	private Course _course;
	private int _maxCapacity=300;
	private Map<Integer, Student> _students = new TreeMap<Integer, Student>();
	private Map<Integer, Professor> _professors = new TreeMap<Integer, Professor>();
	private Map<String, Project> _projects = new TreeMap<String, Project>();
	private Set<Watcher> _watchers = new HashSet<Watcher>();

	public Discipline(String name, Course course){
		_name = name; _course = course;
	}

	public Discipline(String name, Course course, int capacity){
		this(name, course);
		_maxCapacity = capacity;
	}

	public String getName(){
		return _name;
	}

	public void addStudent(Student student) throws DisciplineFullOnStudentsException{
		if (_students.size() > _maxCapacity) throw new DisciplineFullOnStudentsException(_name);
		_students.put(student.getId(),student);
		_watchers.add(student);
	}

	public void addProfessor(Professor professor){
		_professors.put(professor.getId(),professor);
		_watchers.add(professor);
	}

	public boolean containsPerson(int id){
		return _students.containsKey(id) || _professors.containsKey(id);
	}

	public boolean containsProfessor(Professor p){
		return _professors.containsKey(p.getId());
	}

	public boolean containsStudent(Student s){
		return _students.containsKey(s.getId());
	}

	public void addProject(String name, Project p) {
		_projects.put(name, p);
	}

	public boolean containsProject(String name){
		return _projects.containsKey(name);
	}

	public Project getProject(String name){
		return _projects.get(name);
	}

	public Map<String, Project> getProjects() {return _projects;}

	public List<Student> getStudents(){
		List<Student> lst = new LinkedList<Student>();
		lst.addAll(_students.values());
		return lst;
	}

	public int compareTo(Discipline o){
		Sorter sorter = new Sorter();
		return sorter.compare(this.toString(), o.toString());
	}

	public void registerWatcher(Watcher o){
		if (!_watchers.contains(o))	_watchers.add(o);
	}

	public void removeWatcher(Watcher o){
		_watchers.remove(o);
	}

	public void notifyWatchers(String msg){
		Set<Watcher> res = new HashSet<Watcher>();
		res.addAll(_watchers);
		res.addAll(_course.getWatchers());
		for (Watcher o: res) o.update(msg);
	}

	public boolean equals(Object o){
		if (o instanceof Discipline){
			Discipline d = (Discipline) o;
			return _name.equals(d._name);
		}
		return false;
	}

	public String toString(){
		return "* "+_course+" - "+_name;
	}

}

