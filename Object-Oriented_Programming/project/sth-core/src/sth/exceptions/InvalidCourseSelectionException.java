package sth.exceptions;

/** Exception thrown when the selected course is invalid. */
public class InvalidCourseSelectionException extends Exception {

  /** Course name. */
  private String _name;

  /**
   * @param name
   */
  public InvalidCourseSelectionException(String name) {
    _name = name;
  }

  /** @return name */
  public String getName() {
    return _name;
  }

}

