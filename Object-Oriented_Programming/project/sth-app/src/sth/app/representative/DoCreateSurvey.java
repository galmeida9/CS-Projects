package sth.app.representative;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.SchoolManager;
import sth.app.exceptions.DuplicateSurveyException;
import sth.exceptions.InvalidDisciplineSelectionException;
import sth.exceptions.ProjectDoesntExistException;
import sth.exceptions.AlreadySubmitedSurveyException;
import sth.app.exceptions.NoSuchProjectException;
import sth.app.exceptions.NoSuchDisciplineException;

/**
 * 4.5.1. Create survey.
 */
public class DoCreateSurvey extends Command<SchoolManager> {

	private Input<String> _disciplineName;
	private Input<String> _projectName;
  /**
   * @param receiver
   */
  public DoCreateSurvey(SchoolManager receiver) {
    super(Label.CREATE_SURVEY, receiver);
	_disciplineName = _form.addStringInput(Message.requestDisciplineName());
	_projectName = _form.addStringInput(Message.requestProjectName());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException, DuplicateSurveyException, NoSuchProjectException, NoSuchDisciplineException {
	_form.parse();
	try {
		_receiver.createSurvey(_disciplineName.value(), _projectName.value());	
	}  
	catch (AlreadySubmitedSurveyException e) {throw new DuplicateSurveyException(e.getDiscipline(), e.getProject());}
	catch (ProjectDoesntExistException e) {throw new NoSuchProjectException(e.getDiscipline(), e.getProject());}
	catch (InvalidDisciplineSelectionException e) {throw new NoSuchDisciplineException(e.getName());}
  }

}
