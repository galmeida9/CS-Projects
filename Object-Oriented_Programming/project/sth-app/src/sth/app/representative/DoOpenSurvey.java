package sth.app.representative;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.SchoolManager;
import sth.exceptions.OpenSurveyException;
import sth.exceptions.SurveyDoesntExistException;
import sth.exceptions.ProjectDoesntExistException;
import sth.exceptions.InvalidDisciplineSelectionException;
import sth.app.exceptions.NoSurveyException;
import sth.app.exceptions.OpeningSurveyException;
import sth.app.exceptions.NoSuchProjectException;
import sth.app.exceptions.NoSuchDisciplineException;

/**
 * 4.5.3. Open survey.
 */
public class DoOpenSurvey extends Command<SchoolManager> {


	private Input<String> _disciplineName;
	private Input<String> _projectName;
  /**
   * @param receiver
   */
  public DoOpenSurvey(SchoolManager receiver) {
    super(Label.OPEN_SURVEY, receiver);
	_disciplineName = _form.addStringInput(Message.requestDisciplineName());
	_projectName = _form.addStringInput(Message.requestProjectName());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException, NoSurveyException, OpeningSurveyException, NoSuchProjectException, NoSuchDisciplineException {
	_form.parse();
	try {
		_receiver.openSurvey(_disciplineName.value(), _projectName.value());	
	}  
	catch (SurveyDoesntExistException e) {throw new NoSurveyException(e.getDiscipline(), e.getProject());}
	catch (ProjectDoesntExistException e) {throw new NoSuchProjectException(e.getDiscipline(), e.getProject());}
	catch (InvalidDisciplineSelectionException e) {throw new NoSuchDisciplineException(e.getName());}
	catch (OpenSurveyException e) {throw new OpeningSurveyException(e.getDiscipline(), e.getProject());}
  }

}
