package sth.app.person;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Display;
import sth.SchoolManager;


/**
 * 4.2.2. Change phone number.
 */
public class DoChangePhoneNumber extends Command<SchoolManager> {

	private Input<String> _phoneNumber;

  	/**
   	* @param receiver
   	*/
  	public DoChangePhoneNumber(SchoolManager receiver) {
    	super(Label.CHANGE_PHONE_NUMBER, receiver);
		_phoneNumber = _form.addStringInput(Message.requestPhoneNumber());
  	}

  	/** @see pt.tecnico.po.ui.Command#execute() */
  	@Override
  	public final void execute() {
		_form.parse();
		_receiver.changePhoneNumber(_phoneNumber.value());
	  	_display = _display.addLine(_receiver.showPerson());
	  	_display.display();
  	}
}
