package sth.app.teaching;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Display;
import sth.SchoolManager;
import sth.exceptions.InvalidDisciplineSelectionException;
import sth.app.exceptions.NoSuchDisciplineException;


/**
 * 4.3.4. Show course students.
 */
public class DoShowDisciplineStudents extends Command<SchoolManager> {


	private Input<String> _discipline;

  	/**
   	* @param receiver
   	*/
  	public DoShowDisciplineStudents(SchoolManager receiver) {
    	super(Label.SHOW_COURSE_STUDENTS, receiver);
		_discipline = _form.addStringInput(Message.requestDisciplineName());
  	}

  	/** @see pt.tecnico.po.ui.Command#execute() */
  	@Override
  	public final void execute() throws DialogException, NoSuchDisciplineException {
		_form.parse();
		try{
			for(String s: _receiver.showDisciplineStudents(_discipline.value()))
				_display = _display.addLine(s);
			_display.display();
		}
		catch(InvalidDisciplineSelectionException e) { throw new NoSuchDisciplineException(e.getName());}

  	}

}
