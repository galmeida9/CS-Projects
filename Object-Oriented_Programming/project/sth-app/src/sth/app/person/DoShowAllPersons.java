package sth.app.person;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Display;
import sth.SchoolManager;


/**
 * 4.2.3. Show all persons.
 */
public class DoShowAllPersons extends Command<SchoolManager> {

  	/**
   	* @param receiver
   	*/
  	public DoShowAllPersons(SchoolManager receiver) {
    	super(Label.SHOW_ALL_PERSONS, receiver);
  	}

  	/** @see pt.tecnico.po.ui.Command#execute() */
  	@Override
	public final void execute() {
		for (String s: _receiver.showAllPerson()){
			_display = _display.addLine(s);
		}
		_display.display();
  	}

}
