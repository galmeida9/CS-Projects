package sth;

import java.io.Serializable;
import java.util.List;
import java.util.LinkedList;
import java.util.Map;
import java.util.TreeMap;
import java.util.Collections;

class Student extends Person implements Serializable{
	
	private Course _course;
	private Map<String, Discipline> _enrolledDisciplines = new TreeMap<String, Discipline>(new Sorter());
	
	public Student(String name, String telephone, int id){
		super(name, telephone,id);
	}

	public void addDiscipline(Discipline discipline){
		_enrolledDisciplines.put(discipline.getName(), discipline);
	}

	public String getDisciplines(){
		String res = new String();
		for (Discipline d: _enrolledDisciplines.values()){
			res += "\n"+d;
		}
		return res;
	}

	public void addCourse(Course course){
		_course = course;
	}

	public Course getCourse() {
		return _course;
	}

	public boolean containsDiscipline(String name) {
		return _enrolledDisciplines.containsKey(name);
	}

	public Discipline getDiscipline(String name){
		return _enrolledDisciplines.get(name);
	}
}

