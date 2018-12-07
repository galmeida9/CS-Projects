package sth.exceptions;

/** Exception thrown when already a survey doesnt exist. */
public class SurveyDoesntExistException extends Exception {

  /** Discipline and project name. */
  private String _discipline;
  private String _project;

  /**
   * @param project_name
   * @param discipline_name
   */
  public SurveyDoesntExistException(String discipline, String project) {
	_discipline = discipline;
	_project = project;
  }

  /** @return Project name */
  	public String getProject() {
		return _project;
	}

	/** @return Discipline name */
	public String getDiscipline() {
		return _discipline;	
	}

}


