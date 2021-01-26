package sth.exceptions;

/** Exception thrown when the selected project isn't open. */
public class ProjectNotOpenException extends Exception {

  /** Project and discipline names. */
  private String _discipline;
  private String _project;

  /**
   * @param Discipline_name
   * @param Project_name
   */
  public ProjectNotOpenException(String discipline, String project) {
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


