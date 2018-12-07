package sth.app.main;

import java.io.IOException;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import sth.exceptions.FileNotSavedException;
import sth.exceptions.ImportFileException;
import sth.SchoolManager;


/**
 * 4.1.1. Save to file under current name (if unnamed, query for name).
 */
public class DoSave extends Command<SchoolManager> {

  /**
   * @param receiver
   */
  public DoSave(SchoolManager receiver) {
    super(Label.SAVE, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
	public final void execute() {
		String filename;
		try{
			filename = _receiver.hasFile();
		}
		catch (FileNotSavedException e) {
    		Input<String> filename_input = _form.addStringInput(Message.newSaveAs());
			_form.parse();
			filename = filename_input.value();
		}
		try{
		  	_receiver.saveFile(filename);
	  	} catch (ImportFileException e){
		  	e.printStackTrace();
	  	}
  	}

}
