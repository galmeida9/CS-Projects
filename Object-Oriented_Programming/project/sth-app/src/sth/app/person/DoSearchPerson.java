package sth.app.person;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Display;
import sth.SchoolManager;


/**
 * 4.2.4. Search person.
 */
public class DoSearchPerson extends Command<SchoolManager> {

	private	Input<String> _name;
  
  	/**
   	* @param receiver
   	*/
  	public DoSearchPerson(SchoolManager receiver) {
    	super(Label.SEARCH_PERSON, receiver);
    	_name = _form.addStringInput(Message.requestPersonName());
  	}

  	/** @see pt.tecnico.po.ui.Command#execute() */
  	@Override
	public final void execute() {
		_form.parse();
		for (String s: _receiver.showPersonByName(_name.value()))
			_display = _display.addLine(s);
		_display.display();
  	}

}
