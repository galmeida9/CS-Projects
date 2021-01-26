package sth.exceptions;

/**
 * Exception for unsupported operations.
 */
public class OperationUnsupportedException extends Exception {

  /** Class serial number. */
  private static final long serialVersionUID = 201409301048L;
  
  public OperationUnsupportedException(){
  }


  /**
   * @param cause
   */
  public OperationUnsupportedException(Exception cause) {
    super(cause);
  }

}
