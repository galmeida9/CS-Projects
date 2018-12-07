package sth.exceptions;

/** Exception thrown when tries to cancel a finalized survey. */
public class SurveyFinishException extends Exception {

  /** Discipline and project name. */
  private String _discipline;
  private String _project;

  /**
   * @param project_name
   * @param discipline_name
   */
  public SurveyFinishException(String discipline, String project) {
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


