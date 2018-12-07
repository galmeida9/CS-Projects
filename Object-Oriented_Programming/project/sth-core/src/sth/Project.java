package sth;

import java.io.Serializable;
import java.util.Map;
import java.util.TreeMap;
import java.util.List;
import java.util.LinkedList;
import sth.exceptions.ProjectNotOpenException;
import sth.exceptions.OpenSurveyException;

class Project implements Serializable{
	private Discipline _discipline;
	private String _name;
	private String _desc;
	private boolean _isOpen;
	private Survey _survey;
	private Map<Integer, String> _submissions = new TreeMap<Integer, String>();
	private boolean _hasSurvey = false;

	public Project(String name, Discipline discipline) {_name = name; _discipline = discipline; _isOpen = true;}

	public Project(String name, String desc, Discipline discipline) {_name = name; _desc = desc; _discipline = discipline; _isOpen = true;}

	public void submit(String submission, Student s) throws ProjectNotOpenException{
		if (!_isOpen) throw new ProjectNotOpenException(getDisciplineName(), _name);
		_submissions.put(s.getId(), submission);
	}

	public void close() {
		_isOpen = false;
		try {
			if (_hasSurvey)	_survey.open();
		} catch(OpenSurveyException e) {e.printStackTrace();}
	}

	public boolean hasStudentSubmited(Student s) {
		return _submissions.containsKey(s.getId());
	}

	public Discipline getDiscipline() {return _discipline;}

	public String getDisciplineName() {return _discipline.getName();}
	
	public String getName() {return _name;}

	public int numberOfSubmissions() {return _submissions.size();}

	public boolean hasSubmitted(int id) {return _submissions.containsKey(id);}

	public List<String> showSubmissions(){
		List<String> lst = new LinkedList<String>();
		lst.add(getDisciplineName() + " - " + _name);
		for (int id: _submissions.keySet()){
			lst.add("* " + id + " - " + _submissions.get(id));
		}
		return lst;
	}

	public void addSurvey(Survey survey) {
		_hasSurvey = true;
		_survey = survey;
	}

	public boolean containsSurvey() {return _hasSurvey;}

	public void removeSurvey() {_hasSurvey = false;}

	public Survey getSurvey() {return _survey;}

	public boolean isOpen() {return _isOpen;}

	@Override
	public String toString() {return _discipline.getName() + " - " + _name;}
}
