package sth;


import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import sth.exceptions.BadEntryException;
import sth.exceptions.NoSuchPersonIdException;
import sth.exceptions.RepresentativeFullException;
import sth.exceptions.InvalidCourseSelectionException;
import sth.exceptions.InvalidDisciplineSelectionException;
import sth.exceptions.DisciplineFullOnStudentsException;
import sth.exceptions.ProjectAlreadyExistsException;
import sth.exceptions.ProjectDoesntExistException;
import sth.exceptions.ProjectNotOpenException;
import sth.exceptions.AlreadySubmitedSurveyException;
import sth.exceptions.SurveyDoesntExistException;
import sth.exceptions.SurveyFinishException;
import sth.exceptions.NotEmptySurveyException;
import sth.exceptions.OpenSurveyException;
import sth.exceptions.CloseSurveyException;
import sth.exceptions.FinishSurveyException;
import sth.exceptions.OperationUnsupportedException;
import java.io.Serializable;
import java.util.Map;
import java.util.TreeMap;
import java.util.List;
import java.util.LinkedList;
import java.util.Collections;

/**
 * School implementation.
 */
class School implements Serializable {

  	/** Serial number for serialization. */
  	private static final long serialVersionUID = 201810051538L;

	/** School name */
	private String _name = "";

  	/** Initial value for the id counter. */
  	private final static int INITIAL_ID = 100000;

  	/** Map of people, that uses their id for the key. */
  	private Map<Integer, Person> _people = new TreeMap<Integer, Person>();

  	/** Map of people's types, that uses their id for the key. */
	private Map<Integer, String> _types = new TreeMap<Integer, String>();

  	/** Map of students, that uses their id for the key. */
  	private Map<Integer, Student> _students = new TreeMap<Integer, Student>();

  	/** Map of professors, that uses their id for the key. */
  	private Map<Integer, Professor> _professors = new TreeMap<Integer, Professor>();

  
  	/** List of courses in the school. */
  	private Map<String, Course> _courses = new TreeMap<String, Course>();
  
  	/** Id counter. */
  	private int _id = INITIAL_ID;

  	/**
	*
	* Imports database from a file, normaly used at the beggining of the program. 
	*
   	* @param filename
	*			Name of file to import
  	* @throws BadEntryException
   	* @throws IOException
   	* @throws DisciplineFullOnStudentsException
	*
   	*/
	public void importFile(String filename) throws IOException, BadEntryException, DisciplineFullOnStudentsException {
		BufferedReader reader = new BufferedReader(new FileReader(filename));
		String line;
		int current_id = 0, id;
		String current_type="";
		while ((line = reader.readLine()) != null){
			String[] fields = line.split("\\|");
			String first = fields[0];
			if (first.startsWith("#")){
				String strCourse = first.split(" ")[1];
				Course course = new Course(strCourse);
				if (_courses.containsKey(strCourse)) course = _courses.get(strCourse);
				else _courses.put(strCourse, course);
				Discipline discipline = new Discipline(fields[1], course);
				if (course.containsDiscipline(discipline.getName())) discipline = course.getDiscipline(fields[1]);
				else course.addDiscipline(discipline);
				if ( current_id != 0){
					if (current_type.equals("Student")){
						Student s = _students.get(current_id);
						s.addCourse(course);
						s.addDiscipline(discipline);
						course.addStudent(s);
						discipline.addStudent(s);
						if (isRepresentative(current_id)) course.addRepresentative(s);
					}else if (current_type.equals("Professor")){
						Professor p = _professors.get(current_id);
						p.addCourse(course);
						p.addDiscipline(discipline);
						discipline.addProfessor(p);
					}
				}
	
			}
			else if (first.equals("ALUNO")||first.equals("DELEGADO")){
				id = Integer.parseInt(fields[1]); 
				Student student = new Student(fields[3], fields[2], id);
				_people.put(id, student);
				_types.put(id, first); 
				_students.put(id, student);
				current_id = id;
				if (current_id > _id) _id = current_id + 1;				
				current_type = "Student";
			}
			else if (first.equals("DOCENTE")){
				id = Integer.parseInt(fields[1]); 
				Professor professor = new Professor(fields[3], fields[2], id);
				_people.put(id, professor);
				_types.put(id, first); 
				_professors.put(id, professor);
				current_id = id;
				if (current_id > _id) _id = current_id + 1;				
				current_type = "Professor";
	
			}
			else if (first.equals("FUNCIONÁRIO")){
				id = Integer.parseInt(fields[1]); 
				Administrative administrative = new Administrative(fields[3], fields[2], id);
				_people.put(id, administrative);
				_types.put(id, first); 
				current_id = id;
				if (current_id > _id) _id = current_id + 1;							
				current_type = "Funcionário";
			}
		}	
	}
	
	public boolean isStudent(int id){
		String type = _types.get(id);
		return type.equals("ALUNO") || type.equals("DELEGADO");
	}

	public boolean isProfessor(int id){
		String type = _types.get(id);
		return type.equals("DOCENTE");
	}

	public boolean isAdministrative(int id){
		String type = _types.get(id);
		return type.equals("FUNCIONÁRIO");
	}

	public boolean isRepresentative(int id){
		String type = _types.get(id);
		return type.equals("DELEGADO");
	}
	
	/**
	*
  	* Searches a person by id and if it finds it, than it returns the Object
  	*
  	* @param id
	* 			Id of person to be found.
  	* @throws NoSuchPersonIdException
	* @return Person with the initial id.
  	*/
	public Person searchPerson(int id) throws NoSuchPersonIdException {
		if (!_people.containsKey(id)) throw new NoSuchPersonIdException(id);
			return _people.get(id);
  	}

	/**
	*
	* Shows Person.
	*
	* @param person
	*		Person to be shown.
	* @return String with person shown.
	*/
	public String showPerson(Person p){
		String res = new String();
		res += _types.get(p.getId());
		res += p.showPerson();
		res += p.getDisciplines();
		return res;
	}
	
	/**
	*
	* Searches a person by its name on the school and then returns a String array with all the results.
	*
	* @param name
	*			Name of the person to be found.
	* @return A String Array with People
	*
	*/
  	public String[] showPersonByName(String name){
	  	Map<String, String> map = new TreeMap<String, String>(new Sorter());
	  	for (Person p: _people.values()){
		  	if (p.nameContains(name)) map.put(p.getName(),showPerson(p));
	  	}
	  	List<String> lst = new LinkedList<String>();
	  	lst.addAll(map.values());
	  	return lst.toArray(new String[lst.size()]);
  	}

   	/**
	*
	* Returns a String array with all the people in the school.
	*
	* @return A String Array with people
	*
	*/
  	public String[] showAllPerson(){
		List<String> lst = new LinkedList<String>();
		for (Person p: _people.values())
	 		lst.add(showPerson(p));
		return lst.toArray(new String[lst.size()]);	  
  	}

	public String[] sendNotifications(Person p){
		return p.sendNotifications();
	}

   	/**
	*
	* Searches a course by its name in the school and if it finds it, then it returns it.
	*
	* @param name
	*			Name of the course to be found.
	* @return Course or null
	*/
  	private Course getCourse(String name){
		return _courses.get(name);
  	}
	
	/**
	*
	* Finds all the students from a given discipline lectured by a given teacher and returns the results in a String array.
	*
	* @param name
	*			Name of the Discipline
	* @param professor
	*			Professor that asks for this list
	* @throws InvalidDisciplineSelectionException 
	* @return A String Array with students
	*/
	public String[] showDisciplineStudents(String name, int personId) throws InvalidDisciplineSelectionException{
		for (Course course: _courses.values()) {
			if (course.containsDiscipline(name)) {
				Discipline discipline = course.getDiscipline(name); 
				if (discipline.containsPerson(personId)){
					List<String> lst = new LinkedList<String>();
					for (Student s: discipline.getStudents()){
						lst.add(showPerson(s));
					}
					return lst.toArray(new String[lst.size()]);	  
				}
			}
		}
		throw new InvalidDisciplineSelectionException(name);
	}

	/**
	*
	* Creates a project in a given discipline.
	*
	* @param disciplineName
	*			Name of the Discipline to create a project on.
	* @param projectName
	*			Name of the Project to be created.
	* @param professor
	*			Professor creating the project.
	* @throws InvalidDisciplineSelectionException
	* @throws ProjectAlreadyExistsException
	*
	*/
	public void createProject(String disciplineName, String projectName, int personId) throws InvalidDisciplineSelectionException, ProjectAlreadyExistsException {	
		for (Course course: _courses.values()) {
			if (course.containsDiscipline(disciplineName)) {
				Discipline discipline = course.getDiscipline(disciplineName);
				Project project = new Project(projectName, discipline);
				if (discipline.containsPerson(personId)) {
					if (discipline.containsProject(projectName)) 
						throw new ProjectAlreadyExistsException(disciplineName, projectName);
					else{
						discipline.addProject(projectName, project);
						return;
					}
				}
			}
		}
		throw new InvalidDisciplineSelectionException(disciplineName);
	}

	private Project getProject(String disciplineName, String projectName, int personId) throws InvalidDisciplineSelectionException, ProjectDoesntExistException{
		for (Course course: _courses.values()) {
			if (course.containsDiscipline(disciplineName)){
				Discipline discipline = course.getDiscipline(disciplineName);
				if (discipline.containsPerson(personId)) {
					if (discipline.containsProject(projectName))
						return discipline.getProject(projectName);
					else throw new ProjectDoesntExistException(disciplineName, projectName);
				}
				else throw new InvalidDisciplineSelectionException(disciplineName);
			}
		}
		throw new InvalidDisciplineSelectionException(disciplineName);

	}


	/**
	*
	* Closes a project from a given discipline.
	*
	* @param disciplineName
	*			Name of the Discipline to create a project on.
	* @param projectName
	*			Name of the Project to be created.
	* @param professor
	*			Professor creating the project.
	*
	* @throws InvalidDisciplineSelectionException
	* @throws ProjectDoesntExistException
	*
	*/
	public void closeProject(String disciplineName, String projectName, int personId) throws InvalidDisciplineSelectionException, ProjectDoesntExistException, OpenSurveyException {
		Project project = getProject(disciplineName, projectName, personId);
		project.close();
		if (project.containsSurvey()) {
			Survey survey = project.getSurvey();
			survey.open();	
		}
	}

	public void submitProject(String disciplineName, String projectName, String submission, int personId) throws ProjectNotOpenException, ProjectDoesntExistException, InvalidDisciplineSelectionException{
		Project project = getProject(disciplineName, projectName, personId);
		Student student = _students.get(personId);
		project.submit(submission, student);
	}

	public String[] showProjectSubmissions(String disciplineName, String projectName, int personId) throws ProjectDoesntExistException, InvalidDisciplineSelectionException{
		Project project = getProject(disciplineName, projectName, personId);
		List<String> lst = project.showSubmissions();
		return (String[]) lst.toArray(new String[lst.size()]);
	}


	/**
	*
	* Makes a student representative in its course.
	*
	* @param student
	*			Student to become a Representative.
	*
	* @throws RepresentativeFullException
	*
	*/
	public void makeRepresentative(int id) throws RepresentativeFullException {
		Student student = _students.get(id);
		Course course = student.getCourse();
		if (course.isRepFull()) throw new RepresentativeFullException(course.getName());
		course.addRepresentative(student);
		_types.replace(student.getId(), "DELEGADO");
	}

	public void createSurvey(String disciplineName, String projectName, int personId) throws AlreadySubmitedSurveyException, InvalidDisciplineSelectionException, ProjectDoesntExistException{
		Student student = _students.get(personId);
		Course course = student.getCourse();
		if (course.containsDiscipline(disciplineName)){
			Discipline discipline = course.getDiscipline(disciplineName);
			if (discipline.containsProject(projectName)) {
				Project project = discipline.getProject(projectName);
				if (!(project.isOpen())) throw new ProjectDoesntExistException(disciplineName, projectName);
				if (!(project.containsSurvey())) {
					Survey survey = new Survey(project);
					project.addSurvey(survey);	
				}
				else throw new AlreadySubmitedSurveyException(disciplineName, projectName);	
			}				
			else throw new 	ProjectDoesntExistException(disciplineName, projectName);
		}
		else throw new InvalidDisciplineSelectionException(disciplineName);
	}

	public Survey getSurveyByStudent(String disciplineName, String projectName, int personId) throws InvalidDisciplineSelectionException, ProjectDoesntExistException, SurveyDoesntExistException{
		Student student = _students.get(personId);
		Course course = student.getCourse();
		if (course.containsDiscipline(disciplineName)){
			Discipline discipline = course.getDiscipline(disciplineName);
			if (discipline.containsProject(projectName)) {
				Project project = discipline.getProject(projectName);
				if (project.containsSurvey()) {
					Survey survey = project.getSurvey();
					return survey;
				}
				else throw new SurveyDoesntExistException(disciplineName, projectName);	
			}				
			else throw new 	ProjectDoesntExistException(disciplineName, projectName);
		}
		else throw new InvalidDisciplineSelectionException(disciplineName);
	}

	public void cancelSurvey(String disciplineName, String projectName, int personId) throws SurveyDoesntExistException, InvalidDisciplineSelectionException, ProjectDoesntExistException, SurveyFinishException, NotEmptySurveyException{
		Survey survey = getSurveyByStudent(disciplineName, projectName, personId);
		survey.cancel();
	}

	public void openSurvey(String disciplineName, String projectName, int personId) throws SurveyDoesntExistException, InvalidDisciplineSelectionException, ProjectDoesntExistException, OpenSurveyException{
		Survey survey = getSurveyByStudent(disciplineName, projectName, personId);
		survey.open();
	}

	public void closeSurvey(String disciplineName, String projectName, int personId) throws SurveyDoesntExistException, InvalidDisciplineSelectionException, ProjectDoesntExistException, CloseSurveyException{
		Survey survey = getSurveyByStudent(disciplineName, projectName, personId);
		survey.close();
	}

	public void finishSurvey(String disciplineName, String projectName, int personId) throws SurveyDoesntExistException, InvalidDisciplineSelectionException, ProjectDoesntExistException, FinishSurveyException{
		Survey survey = getSurveyByStudent(disciplineName, projectName, personId);
		survey.finish();
	}

	public void submitSurvey(String disciplineName, String projectName, int hours, String comment, int personId) throws SurveyDoesntExistException, InvalidDisciplineSelectionException, ProjectDoesntExistException{
		Survey survey = getSurveyByStudent(disciplineName, projectName, personId);
		survey.submit(hours, comment, personId);
	}

	public String showSurveyResults(String disciplineName, String projectName, Person person) throws SurveyDoesntExistException, InvalidDisciplineSelectionException, ProjectDoesntExistException {
		try{
			if (person.containsDiscipline(disciplineName)) {
				Discipline discipline = person.getDiscipline(disciplineName);
				if (discipline.containsProject(projectName)) {
					Project project = discipline.getProject(projectName);
					if (isStudent(person.getId()) && (!project.hasSubmitted(person.getId()))) throw new  ProjectDoesntExistException(disciplineName, projectName);
					if (project.containsSurvey()) {
						Survey survey = project.getSurvey();
						return survey.showResults(this, person.getId(), false);					
					}
					else throw new SurveyDoesntExistException(disciplineName, projectName);
				}
				else throw new  ProjectDoesntExistException(disciplineName, projectName);			
			}
			else throw new InvalidDisciplineSelectionException(disciplineName);	
		}
		catch (OperationUnsupportedException e){ e.printStackTrace();}
		return "";
	}
	
	public String[] showSurveyResultsRepresentative(String disciplineName, Student student) throws InvalidDisciplineSelectionException {
		Course course = student.getCourse();
		if (course.containsDiscipline(disciplineName)) {
			Discipline discipline = course.getDiscipline(disciplineName);
	  		Map<String, Project> projects = discipline.getProjects();
			Map<String, String> map = new TreeMap<String, String>(new Sorter());
			for (Project p: projects.values()){
				if (p.containsSurvey()) {
					Survey survey = p.getSurvey();
					map.put(p.getName(), survey.showResults(this, student.getId(), true));
				}
			}
	  		List<String> lst = new LinkedList<String>();
	  		lst.addAll(map.values());
	  		return lst.toArray(new String[lst.size()]);
		}
		else throw new InvalidDisciplineSelectionException(disciplineName);
	}
}
