package sth;

import java.io.Serializable;
import java.util.List;
import java.util.LinkedList;
import java.util.Map;
import java.util.TreeMap;
import java.util.Collections;

class Professor extends Person implements Serializable{

	private List<Discipline> _disciplines = new LinkedList<Discipline>();
	private Map<String, Course> _courses = new TreeMap<String, Course>();

	public Professor(String name, String telephone, int id){
		super(name, telephone, id);
	}

	public void addCourse(Course c){
		_courses.put(c.getName(),c);
	}

	public void addDiscipline(Discipline d){
		_disciplines.add(d);
	}

	public String getDisciplines(){
		String res = new String();
		Collections.sort(_disciplines);
		for (int i=0; i<_disciplines.size(); i++){
			Discipline d = _disciplines.get(i);
			res += "\n"+d;
		}
		return res;
	}
	
	public boolean containsDiscipline(String name) {
		for (int i = 0; i < _disciplines.size(); i++) {
			Discipline discipline = _disciplines.get(i);
			String disciplineName = discipline.getName();
			if (disciplineName.equals(name)) return true;
		}
		return false;
	}

	public Discipline getDiscipline(String name){
		for (int i = 0; i < _disciplines.size(); i++) {
			Discipline discipline = _disciplines.get(i);
			String disciplineName = discipline.getName();
			if (disciplineName.equals(name)) return discipline;
		}
		return null;
	}
}
