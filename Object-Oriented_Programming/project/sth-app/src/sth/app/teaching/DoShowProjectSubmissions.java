package sth.app.teaching;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Display;
import pt.tecnico.po.ui.Input;
import sth.app.exceptions.NoSuchDisciplineException;
import sth.app.exceptions.NoSuchProjectException;
import sth.SchoolManager;
import sth.exceptions.InvalidDisciplineSelectionException;
import sth.exceptions.ProjectDoesntExistException;

/**
 * 4.3.3. Show project submissions.
 */
public class DoShowProjectSubmissions extends Command<SchoolManager> {

	private Input<String> _discipline;
	private Input<String> _project;

	/**
   	* @param receiver
   	*/
	public DoShowProjectSubmissions(SchoolManager receiver) {
		super(Label.SHOW_PROJECT_SUBMISSIONS, receiver);
		_discipline = _form.addStringInput(Message.requestDisciplineName());
		_project = _form.addStringInput(Message.requestProjectName());
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() throws DialogException {
		_form.parse();
		try{
			for (String s: _receiver.showProjectSubmissions(_discipline.value(), _project.value()))
				_display = _display.addLine(s);
			_display.display();
		}
		catch (InvalidDisciplineSelectionException e){ throw new NoSuchDisciplineException(_discipline.value());}
		catch (ProjectDoesntExistException e){ throw new NoSuchProjectException(_discipline.value(), _project.value());}
	}
}
