package sth;

import java.io.FileInputStream;
import java.io.BufferedInputStream;
import java.io.ObjectInputStream;

import java.io.FileOutputStream;
import java.io.BufferedOutputStream;
import java.io.ObjectOutputStream;

import java.io.IOException;
import java.io.FileNotFoundException;
import sth.exceptions.BadEntryException;
import sth.exceptions.ImportFileException;
import sth.exceptions.NoSuchPersonIdException;
import sth.exceptions.InvalidDisciplineSelectionException;
import sth.exceptions.ProjectAlreadyExistsException;
import sth.exceptions.ProjectDoesntExistException;
import sth.exceptions.ProjectNotOpenException;
import sth.exceptions.RepresentativeFullException;
import sth.exceptions.DisciplineFullOnStudentsException;
import sth.exceptions.FileNotSavedException;
import sth.exceptions.AlreadySubmitedSurveyException;
import sth.exceptions.SurveyDoesntExistException;
import sth.exceptions.NotEmptySurveyException;
import sth.exceptions.SurveyFinishException;
import sth.exceptions.OpenSurveyException;
import sth.exceptions.CloseSurveyException;
import sth.exceptions.FinishSurveyException;


/**
 * The façade class.
 */
public class SchoolManager {

	private School _school = new School();	
	private Person _personLogged;
	private String _filename="";
	private boolean _dirty=true;
	
  	/**
   	* @param datafile
   	* @throws ImportFileException
   	* @throws InvalidCourseSelectionException
   	*/
  	public void importFile(String datafile) throws ImportFileException {
    	try {
      	_school.importFile(datafile);
    	} catch (IOException | BadEntryException e) {
      	throw new ImportFileException(e);
    	} catch (DisciplineFullOnStudentsException e){
		}

  	}

	public void loadFile(String filename) throws ClassNotFoundException, FileNotFoundException, IOException, NoSuchPersonIdException{
		ObjectInputStream ois = new ObjectInputStream(new BufferedInputStream(new FileInputStream(filename)));
		School school_aux = (School) ois.readObject();
		ois.close();
		Person person_aux = school_aux.searchPerson(_personLogged.getId());
		_school = school_aux;
		_personLogged = person_aux;	
		_filename = filename;
	}

	public String hasFile() throws FileNotSavedException{
		if (_filename.equals("")) throw new FileNotSavedException();
		return _filename;
	}

	public void saveFile(String filename) throws ImportFileException{
		if (!_dirty) return;
		try{
			ObjectOutputStream oos = new ObjectOutputStream(new BufferedOutputStream(new FileOutputStream(filename)));
			oos.writeObject(_school);
			oos.close();
			_filename = filename;
			_dirty = false;
		} catch (IOException e){ throw new ImportFileException(e);}
	}
 


  	/**
   	* @param id
   	* @throws NoSuchPersonIdException
   	*/
  	public void login(int id) throws NoSuchPersonIdException {
		_personLogged =	_school.searchPerson(id);
  	}

  	/**
   	* @return true when the currently logged in person is an administrative
   	*/
  	public boolean hasAdministrative() {
    	return _school.isAdministrative(_personLogged.getId());
 	}

  	/**
   	* @return true when the currently logged in person is a professor
   	*/
  	public boolean hasProfessor() {
    	return _school.isProfessor(_personLogged.getId());
  	}

  	/**
   	* @return true when the currently logged in person is a student
   	*/
  	public boolean hasStudent() {
    	return _school.isStudent(_personLogged.getId());
  	}

  	/**
   	* @return true when the currently logged in person is a representative
   	*/
 	public boolean hasRepresentative() {
    	return _school.isRepresentative(_personLogged.getId());
  	}

  	public String showPerson(){
	  	return _school.showPerson(_personLogged);
  	}

  	public String[] showPersonByName(String name){
	  	return _school.showPersonByName(name);
  	}

  	public String[] showAllPerson(){
		return _school.showAllPerson();
  	}
	
	public void changePhoneNumber(String number) {
	  _personLogged.changeTelephone(number);
	  _dirty = true;
	}

	public String[] sendNotifications(){
		_dirty = true;
		return _school.sendNotifications(_personLogged);
	}

	public String[] showDisciplineStudents(String name) throws InvalidDisciplineSelectionException {
		return _school.showDisciplineStudents(name, _personLogged.getId());
	}
	
	public void createProject(String discipline, String project) throws InvalidDisciplineSelectionException, ProjectAlreadyExistsException{
		_school.createProject(discipline, project, _personLogged.getId());
		_dirty = true;
	}

	public void closeProject(String discipline, String project) throws InvalidDisciplineSelectionException, ProjectDoesntExistException, OpenSurveyException{
		_school.closeProject(discipline, project, _personLogged.getId());
		_dirty = true;
	}
	
	public void submitProject(String discipline, String project, String submission) throws InvalidDisciplineSelectionException, ProjectDoesntExistException, ProjectNotOpenException{
		_school.submitProject(discipline, project, submission, _personLogged.getId());
		_dirty = true;
	}

	public String[] showProjectSubmissions(String discipline, String project) throws InvalidDisciplineSelectionException, ProjectDoesntExistException{
		return _school.showProjectSubmissions(discipline, project, _personLogged.getId());
	}

	public void makeRepresentative() throws RepresentativeFullException {
		_school.makeRepresentative(_personLogged.getId());
		_dirty = true;
	}

	public void createSurvey(String disciplineName, String projectName) throws AlreadySubmitedSurveyException, InvalidDisciplineSelectionException, ProjectDoesntExistException {
		_school.createSurvey(disciplineName, projectName, _personLogged.getId());		
		_dirty = true;
	}

	public void cancelSurvey(String disciplineName, String projectName) throws SurveyDoesntExistException, InvalidDisciplineSelectionException, ProjectDoesntExistException, SurveyFinishException, NotEmptySurveyException {
		_school.cancelSurvey(disciplineName, projectName, _personLogged.getId());	
		_dirty = true;
	}

	public void openSurvey(String disciplineName, String projectName) throws SurveyDoesntExistException, InvalidDisciplineSelectionException, ProjectDoesntExistException, OpenSurveyException {
		_school.openSurvey(disciplineName, projectName, _personLogged.getId());
		_dirty = true;
	}
	public void closeSurvey(String disciplineName, String projectName) throws SurveyDoesntExistException, InvalidDisciplineSelectionException, ProjectDoesntExistException, CloseSurveyException{
		_school.closeSurvey(disciplineName, projectName, _personLogged.getId());	
		_dirty = true;
	}
	public void finishSurvey(String disciplineName, String projectName) throws SurveyDoesntExistException, InvalidDisciplineSelectionException, ProjectDoesntExistException, FinishSurveyException{
		_school.finishSurvey(disciplineName, projectName, _personLogged.getId());	
		_dirty = true;
	}

	public void submitSurvey(String disciplineName, String projectName, int hours, String comment) throws SurveyDoesntExistException, InvalidDisciplineSelectionException, ProjectDoesntExistException{
		_school.submitSurvey(disciplineName, projectName, hours, comment, _personLogged.getId());
		_dirty = true;
	}

	public String showSurveyResults(String disciplineName, String projectName) throws SurveyDoesntExistException, InvalidDisciplineSelectionException, ProjectDoesntExistException {
		return _school.showSurveyResults(disciplineName, projectName, _personLogged);	
	}

	public String[] showSurveyResultsRepresentative(String disciplineName) throws InvalidDisciplineSelectionException {
		return _school.showSurveyResultsRepresentative(disciplineName, (Student) _personLogged);
	}
}
