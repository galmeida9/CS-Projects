package sth.exceptions;

/** Exception thrown when its not possible to have more representatives. */
public class RepresentativeFullException extends Exception {

  /** Course name. */
  private String _course;

  /**
   * @param Course_name
   */
  public RepresentativeFullException(String name) {
    _course = name;
  }

  /** @return Course name */
  public String getCourse() {
    return _course;
  }

}

