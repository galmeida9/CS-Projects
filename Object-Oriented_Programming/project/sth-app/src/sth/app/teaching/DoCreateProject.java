package sth.app.teaching;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.SchoolManager;
import sth.app.exceptions.NoSuchDisciplineException;
import sth.exceptions.InvalidDisciplineSelectionException;
import sth.exceptions.ProjectAlreadyExistsException;
import sth.app.exceptions.DuplicateProjectException;


/**
 * 4.3.1. Create project.
 */
public class DoCreateProject extends Command<SchoolManager> {

	private Input<String> _discipline;
	private Input<String> _project;
  /**
   * @param receiver
   */
  public DoCreateProject(SchoolManager receiver) {
    super(Label.CREATE_PROJECT, receiver);
	_discipline = _form.addStringInput(Message.requestDisciplineName());
	_project = _form.addStringInput(Message.requestProjectName());	
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException, NoSuchDisciplineException, DuplicateProjectException {
	_form.parse();
	try{
		_receiver.createProject(_discipline.value(), _project.value());
	}
	catch (InvalidDisciplineSelectionException e) {throw new NoSuchDisciplineException(e.getName());}
	catch (ProjectAlreadyExistsException e) {throw new DuplicateProjectException(e.getDiscipline(), e.getProject());}
  }

}
