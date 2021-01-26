package sth.exceptions;

/** Exception thrown when the created project already exists */
public class ProjectAlreadyExistsException extends Exception {

  /** Project name and discipline. */
  private String _discipline;
  private String _project;

  /**
   * @param discipline_name
   * @param project_name
   */
  public ProjectAlreadyExistsException(String discipline, String project) {
    _discipline = discipline;
	_project = project;
  }

  /** @return Discipline name */
  public String getDiscipline() {
    return _discipline;
  }

  /** @return Project name */
  	public String getProject() {
		return _project;
	}

}


