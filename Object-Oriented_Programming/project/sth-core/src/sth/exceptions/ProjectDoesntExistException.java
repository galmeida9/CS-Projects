package sth.exceptions;

/** Exception thrown when the selected Project doesnt exist. */
public class ProjectDoesntExistException extends Exception {

  /** Project and discipline names. */
  private String _discipline;
  private String _project;

  /**
   * @param Discipline_name
   * @param Project_name
   */
  public ProjectDoesntExistException(String discipline, String project) {
	_discipline = discipline;
	_project = project;
  }

  /** @return Discipline name */
  public String getDiscipline() {
    return _discipline;
  }

  /** @return Project name */
  	public String getProject(){
		return _project;
	}

}


