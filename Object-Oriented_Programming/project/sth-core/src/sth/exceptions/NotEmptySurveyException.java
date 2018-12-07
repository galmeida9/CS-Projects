package sth.exceptions;

/** Exception thrown when survey its not empty. */
public class NotEmptySurveyException extends Exception {

  /** Discipline and project name. */
  private String _discipline;
  private String _project;

  /**
   * @param project_name
   * @param discipline_name
   */
  public NotEmptySurveyException(String discipline, String project) {
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


