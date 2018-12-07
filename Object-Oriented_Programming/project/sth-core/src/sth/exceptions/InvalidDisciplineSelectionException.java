package sth.exceptions;

/** Exception thrown when the selected discipline is invalid. */
public class InvalidDisciplineSelectionException extends Exception {

  /** Discipline name. */
  private String _name;

  /**
   * @param name
   */
  public InvalidDisciplineSelectionException(String name) {
	_name = name;
  }

  /** @return name */
  public String getName() {
    return _name;
  }

}


