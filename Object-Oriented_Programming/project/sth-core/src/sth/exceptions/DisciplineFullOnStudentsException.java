package sth.exceptions;

/** Exception thrown when the selected Discipline doesn't accept more students.. */
public class DisciplineFullOnStudentsException extends Exception {

  /** Discipline name. */
  private String _name;

  /**
   * @param name
   */
  public DisciplineFullOnStudentsException(String name) {
	_name = name;
  }

  /** @return name */
  public String getName() {
    return _name;
  }

}



