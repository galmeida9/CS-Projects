package sth;

import java.io.Serializable;
import java.util.List;
import java.util.LinkedList;
import java.util.Set;
import java.util.HashSet;
import sth.exceptions.ProjectDoesntExistException;
import sth.exceptions.AlreadySubmitedSurveyException;
import sth.exceptions.NotEmptySurveyException;
import sth.exceptions.SurveyFinishException;
import sth.exceptions.OpenSurveyException;
import sth.exceptions.CloseSurveyException;
import sth.exceptions.FinishSurveyException;
import sth.exceptions.SurveyDoesntExistException;

class Survey implements Serializable{
	private SurveyState _state = new CreatedState(this);
	private int _nAnswers = 0;
	private int _sumHours = 0;
	private int _minHours = -1;
	private int _maxHours = 0;
	private List<String> _comments = new LinkedList<String>();
	private Set<Integer> _students = new HashSet<Integer>();
	/*list of students that have answered the suvery, this allows us to see if the students have already
	answered the survey*/
	private Project _proj;

	public Survey(Project project) {_proj = project;}

	abstract class SurveyState implements Serializable{
		public void cancel() throws NotEmptySurveyException, SurveyFinishException{}
		public void open () throws OpenSurveyException{}
		public void close() throws CloseSurveyException {}
		public void finish() throws FinishSurveyException {}
		public Survey getSurvey() {return Survey.this;}
		public void setState(SurveyState state) {_state = state;}
		public boolean noSubmissions() {
			 return _comments.size() == 0;	
		}
		public void submit(int hours, String comment, int id) throws SurveyDoesntExistException, ProjectDoesntExistException {}
		public abstract String getState();
		public abstract String showResults(School school, int id, boolean representative);
		public Project getProject() {return _proj;}
		public Discipline getDiscipline() {return _proj.getDiscipline();}
		public int getAnswers() {return _nAnswers;}
		public int getSumHours() {return _sumHours;}
		public int getMinHours() {return _minHours;}
		public int getMaxHours() {return _maxHours;}
	}

	public boolean surveyContainsStudent(int id) {return _students.contains(id);}
	public void surveyAddStudent(int id) {_students.add(id);}
	public void surveyAddComment(String comment) {_comments.add(comment);}
	public int getAnswers() {return _nAnswers;}
	public int getSumHours() {return _sumHours;}
	public int getMinHours() {return _minHours;}
	public int getMaxHours() {return _maxHours;}
	public void incrementAnswers() {_nAnswers++;}
	public void addHours(int value) {_sumHours += value;}
	public void setMinHours(int value) {_minHours = value;}
	public void setMaxHours(int value) {_maxHours = value;}
	public void submit(int hours, String comment, int id) throws SurveyDoesntExistException, ProjectDoesntExistException {_state.submit(hours, comment, id);}
	public String showResults(School school, int id, boolean representative) {return _state.showResults(school, id, representative);}
	public void cancel() throws NotEmptySurveyException, SurveyFinishException {_state.cancel();}
	public void open() throws OpenSurveyException {_state.open();}
	public void close() throws CloseSurveyException {_state.close();}
	public void finish() throws FinishSurveyException {_state.finish();}
	public void remove() {_proj.removeSurvey();}
	public String getState() {return _state.getState();}
	public void notifyPeople() {}

}

class CreatedState extends Survey.SurveyState implements Serializable{
	public CreatedState(Survey survey) {survey.super();}
	public void cancel() throws NotEmptySurveyException, SurveyFinishException {getSurvey().remove();}
	public void open() throws OpenSurveyException {
		if (getProject().isOpen()) throw new OpenSurveyException(getDiscipline().getName(), getProject().getName());
		setState(new OpenState(getSurvey()));
		getDiscipline().notifyWatchers("Pode preencher inquérito do projecto " + getProject().getName() + " da disciplina " + getProject().getDisciplineName());
	}
	public void close() throws CloseSurveyException {
		throw new CloseSurveyException(getDiscipline().getName(), getProject().getName());
	}
	public void finish() throws FinishSurveyException {
		throw new FinishSurveyException(getDiscipline().getName(), getProject().getName());
	}
	public String getState() {return "CREATED";}
	public String showResults(School school, int id, boolean representative) {
		String res = getProject().toString() + " (por abrir)";
		return res;
	}
	public void submit(int hours, String comment, int id) throws SurveyDoesntExistException, ProjectDoesntExistException {
	if (!(getProject().hasSubmitted(id))) throw new ProjectDoesntExistException(getProject().getDisciplineName(), getProject().getName());
	throw new SurveyDoesntExistException(getDiscipline().getName() , getProject().getName());
	}
}

class OpenState extends Survey.SurveyState implements Serializable{
	public OpenState(Survey survey) {survey.super(); }
	public void cancel() throws NotEmptySurveyException, SurveyFinishException{
		if (noSubmissions()) getSurvey().remove();
		else {
			throw new NotEmptySurveyException(getDiscipline().getName(), getProject().getName());
		}
	}
	public void close() throws CloseSurveyException {setState(new CloseState(getSurvey()));}
	public void finish() throws FinishSurveyException {
		throw new FinishSurveyException(getDiscipline().getName(), getProject().getName());
	}	
	public String getState() {return "OPEN";}
	public String showResults(School school, int id, boolean representative) {
		String res = getProject().toString() + " (aberto)";
		return res;
	}
	public void submit(int hours, String comment, int id) throws SurveyDoesntExistException, ProjectDoesntExistException {
		if (!getSurvey().surveyContainsStudent(id)) {
			if (!(getProject().hasSubmitted(id))) throw new ProjectDoesntExistException(getProject().getDisciplineName(), getProject().getName());
			Survey survey = getSurvey();
			survey.addHours(hours);
			if (hours > survey.getMaxHours()) survey.setMaxHours(hours);
			if (survey.getMinHours() == -1) survey.setMinHours(hours);
			else if (hours < survey.getMinHours()) survey.setMinHours(hours);
			survey.surveyAddComment(comment);
			survey.surveyAddStudent(id);
			survey.incrementAnswers();
		}
	}
}

class CloseState extends Survey.SurveyState implements Serializable{
	public CloseState(Survey survey) {survey.super();}
	public void cancel() throws NotEmptySurveyException, SurveyFinishException {setState(new OpenState(getSurvey()));}
	public void finish() throws FinishSurveyException {
		setState(new FinishState(getSurvey()));
		getDiscipline().notifyWatchers("Resultados do inquérito do projecto " + getProject().getName() + " da disciplina " + getProject().getDisciplineName());
	}	
	public String getState() {return "CLOSED";}
	public String showResults(School school, int id, boolean representative) {
		String res = getProject().toString() + " (fechado)";
		return res;
	}
	public void submit(int hours, String comment, int id) throws SurveyDoesntExistException, ProjectDoesntExistException {
	if (!(getProject().hasSubmitted(id))) throw new ProjectDoesntExistException(getProject().getDisciplineName(), getProject().getName());
	throw new SurveyDoesntExistException(getDiscipline().getName() , getProject().getName());
	}
}

class FinishState extends Survey.SurveyState implements Serializable{
	public FinishState(Survey survey) {survey.super();}
	public void cancel() throws SurveyFinishException, NotEmptySurveyException {
		throw new SurveyFinishException(getDiscipline().getName(), getProject().getName());
	}
	public void open() throws OpenSurveyException {
		throw new OpenSurveyException(getDiscipline().getName(), getProject().getName());
	}
	public void close() throws CloseSurveyException {
		throw new CloseSurveyException(getDiscipline().getName(), getProject().getName());
	}
	public String getState() {return "FINISHED";}	
	public String showResults(School school, int id, boolean representative) {
		String res = new String();
		String firstLine = getProject().toString();	
		res += firstLine;
		int averageTime = 0;
		if (getAnswers() != 0) averageTime = (getSumHours() / getAnswers());
		if (school.isStudent(id) && (!representative)) {
			String nAnswers = " * Número de respostas: " + getAnswers();
			String time = " * Tempo médio (horas): " + averageTime;
			res += "\n" + nAnswers + "\n" + time;
		}
		else if (school.isProfessor(id)) {
			int minHours = getMinHours();
			if (minHours == -1) minHours = 0;
			String nSubmissions = " * Número de submissões: " + getProject().numberOfSubmissions(); 
			String nAnswers = " * Número de respostas: " + getAnswers();
			String time = " * Tempos de resolução (horas) (mínimo, médio, máximo): " + minHours + ", " + averageTime + ", " + getMaxHours();
			res += "\n" + nSubmissions + "\n" + nAnswers + "\n" + time;
		}
		else if (school.isRepresentative(id) && representative) {
			res += " - " + getAnswers() + " respostas - " + averageTime + " horas";
		}
		return res;
		
	}
	
	public void submit(int hours, String comment, int id) throws SurveyDoesntExistException, ProjectDoesntExistException {
	if (!(getProject().hasSubmitted(id))) throw new ProjectDoesntExistException(getProject().getDisciplineName(), getProject().getName());
	throw new SurveyDoesntExistException(getDiscipline().getName() , getProject().getName());
	}

}

