package sth.exceptions;

/** Exception thrown when already submited survey one time. */
public class AlreadySubmitedSurveyException extends Exception {

  /** Discipline and project name. */
  private String _discipline;
  private String _project;

  /**
   * @param project_name
   * @param discipline_name
   */
  public AlreadySubmitedSurveyException(String discipline, String project) {
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


