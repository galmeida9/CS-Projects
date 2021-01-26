package sth.app.teaching;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.SchoolManager;
import sth.exceptions.InvalidDisciplineSelectionException;
import sth.exceptions.ProjectDoesntExistException;
import sth.app.exceptions.NoSuchDisciplineException;
import sth.app.exceptions.NoSuchProjectException;
import sth.exceptions.OpenSurveyException;

/**
 * 4.3.2. Close project.
 */
public class DoCloseProject extends Command<SchoolManager> {

	private Input<String> _project;
	private Input<String> _discipline;
  /**
   * @param receiver
   */
  public DoCloseProject(SchoolManager receiver) {
    super(Label.CLOSE_PROJECT, receiver);
	_discipline = _form.addStringInput(Message.requestDisciplineName());
	_project = _form.addStringInput(Message.requestProjectName());	
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException, NoSuchDisciplineException, NoSuchProjectException {
	_form.parse();
	try{
		_receiver.closeProject(_discipline.value(), _project.value());
	}
	catch (InvalidDisciplineSelectionException e) {throw new NoSuchDisciplineException(e.getName());}
	catch (ProjectDoesntExistException e) {throw new NoSuchProjectException(e.getDiscipline(), e.getProject());}
	catch (OpenSurveyException e) {e.printStackTrace();}
  }

}
