class UAT_Interpreter_Base():
  def __init__(self): 
    self.keyword_map = {} 
    self.load_keyword_funcs()
    self.successful_checks = 0
    self.failed_checks = 0
    self.line_num = None
    self.ansi_code_error = '\033[91m'
    self.ansi_code_success = '\033[92m'
    self.ansi_code_warning = '\033[930m'
    self.ansi_code_clear = '\033[0m'
  
  def load_file(self, filename):
    with open(filename, 'r') as in_fp:
      self.line_num = 1
      for line in in_fp:
        self.process_line(line)
        self.line_num += 1

  # Fill out keyword_map which links keyword -> function to call
  def load_keyword_funcs(self):
    self.keyword_map['LOAD_GENOME'] = self.load_genome_raw
    self.keyword_map['SET_REG'] = self.set_reg_raw
    self.keyword_map['SET_IP'] = self.set_ip_raw
    self.keyword_map['SET_RH'] = self.set_rh_raw
    self.keyword_map['SET_WH'] = self.set_wh_raw
    self.keyword_map['SET_FH'] = self.set_fh_raw
    self.keyword_map['EXECUTE'] = self.execute_raw
    self.keyword_map['CHECK_REG'] = self.check_reg_raw
    self.keyword_map['CHECK_IP'] = self.check_ip_raw
    self.keyword_map['CHECK_RH'] = self.check_rh_raw
    self.keyword_map['CHECK_WH'] = self.check_wh_raw
    self.keyword_map['CHECK_FH'] = self.check_fh_raw
    self.keyword_map['CHECK_STACK_A'] = self.check_stack_a_raw
    self.keyword_map['CHECK_STACK_B'] = self.check_stack_b_raw

  def process_line(self, line):
    if '//' in line:
      line = line.split('//')[0]
    line = line.strip()
    if len(line) == 0:
      return
    line_parts = line.split(' ')
    if len(line_parts) < 2: 
      self.throw_line_error('Invalid line')
    keyword = line_parts[0].strip()
    func = self.keyword_map[keyword]
    func(line_parts[1:])

  def print_results(self): 
    print('################ RESULTS #################')
    print(self.ansi_code_success +'Successful checks: ' + str(self.successful_checks) + self.ansi_code_clear)
    if self.failed_checks > 0:
      print(self.ansi_code_error + 'Failed checks: ' + str(self.failed_checks) + self.ansi_code_clear)
    else:
      print(self.ansi_code_success + 'Failed checks: ' + str(self.failed_checks) + self.ansi_code_clear)

  def reset(self):
    self.successful_checks = 0
    self.failed_checks = 0

  def throw_line_error(self, error_str):
    print(self.ansi_code_error + 'Error on line #' + str(self.line_num) + self.ansi_code_clear)
    print(self.ansi_code_error + error_str + self.ansi_code_clear)
 
  def load_genome_raw(self, line_parts):
    if len(line_parts) != 1: 
      error_str = 'LOAD_GENOME expects exactly one argument (the genome to load)\n'
      error_str += 'Given: LOAD_GENOME '
      for s in line_parts:
        error_str += s + ' '
      self.throw_line_error(error_str)
    genome = line_parts[0].strip().strip('"')
    self.load_genome(genome)
  
  def set_reg_raw(self, line_parts):
    if len(line_parts) != 2:
      error_str = 'SET_REG expects exactly two arguments (the register character and value)'
      error_str += 'Given: SET_REG '
      for s in line_parts:
        error_str += s + ' '
      self.throw_line_error(error_str)
    reg_char = line_parts[0].strip(' ').strip("'")
    reg_val = int(line_parts[1])
    self.set_reg(reg_char, reg_val)
  
  def set_ip_raw(self, line_parts):
    if len(line_parts) != 1:
      error_str = 'SET_IP expects exactly one argument (the value to set the IP to)'
      self.throw_line_error(error_str)
    val = int(line_parts[0].strip(' '))
    self.set_ip(val)
  
  def set_rh_raw(self, line_parts):
    if len(line_parts) != 1:
      error_str = 'SET_RH expects exactly one argument (the value to set the RH to)'
      self.throw_line_error(error_str)
    val = int(line_parts[0].strip(' '))
    self.set_rh(val)

  def set_wh_raw(self, line_parts):
    if len(line_parts) != 1:
      error_str = 'SET_WH expects exactly one argument (the value to set the RH to)'
      self.throw_line_error(error_str)
    val = int(line_parts[0].strip(' '))
    self.set_wh(val)
  
  def set_fh_raw(self, line_parts):
    if len(line_parts) != 1:
      error_str = 'SET_FH expects exactly one argument (the value to set the RH to)'
      self.throw_line_error(error_str)
    val = int(line_parts[0].strip(' '))
    self.set_fh(val)

  def execute_raw(self, line_parts):
    if len(line_parts) != 1:
      error_str = 'EXECUTE expects exactly one argument, the number of instructions to execute'
      self.throw_line_error(error_str)
    inst_count = int(line_parts[0])
    self.execute(inst_count)

  def check_reg_raw(self, line_parts):
    if len(line_parts) != 2:
      error_str ='CHECK_REG expects exactly two arguments (the register char and value)'
      self.throw_line_error(error_str)
    reg_char = line_parts[0].strip(' ').strip("'")
    if reg_char not in 'abc':
      error_str = 'CHECK_REG given character: "' + reg_char + '"! Only a, b, and c are supported!'
      self.throw_line_error(error_str)
    expected_reg_val = int(line_parts[1])
    self.check_reg(reg_char, expected_reg_val)
  
  def check_ip_raw(self, line_parts):
    if len(line_parts) != 1:
      error_str = 'CHECK_IP expects exactly one argument (the value to check the IP against)'
      self.throw_line_error(error_str)
    expected_val = int(line_parts[0])
    self.check_ip(expected_val)
  
  def check_rh_raw(self, line_parts):
    if len(line_parts) != 1:
      error_str = 'CHECK_Rh expects exactly one argument (the value to check the IP against)'
      self.throw_line_error(error_str)
    expected_val = int(line_parts[0])
    self.check_rh(expected_val)
  
  def check_wh_raw(self, line_parts):
    if len(line_parts) != 1:
      error_str = 'CHECK_WH expects exactly one argument (the value to check the IP against)'
      self.throw_line_error(error_str)
    expected_val = int(line_parts[0])
    self.check_wh(expected_val)
  
  def check_fh_raw(self, line_parts):
    if len(line_parts) != 1:
      error_str = 'CHECK_FH expects exactly one argument (the value to check the IP against)'
      self.throw_line_error(error_str)
    expected_val = int(line_parts[0])
    self.check_fh(expected_val)

  def check_stack_a_raw(self, line_parts):
    if len(line_parts) != 2:
      error_str = 'CHECK_STACK_A expects exactly one argument (the value to check the IP against)'
      self.throw_line_error(error_str)
    idx = int(line_parts[0])
    expected_val = int(line_parts[1])
    self.check_stack_a(idx, expected_val)

  def check_stack_b_raw(self, line_parts):
    if len(line_parts) != 2:
      error_str = 'CHECK_STACK_B expects exactly one argument (the value to check the IP against)'
      self.throw_line_error(error_str)
    idx = int(line_parts[0])
    expected_val = int(line_parts[1])
    self.check_stack_b(idx, expected_val)

  def trigger_undefined_keyword(self, keyword):
    print(self.ansi_code_error + 'Error! ' + keyword + ' has not been defined in the interpreter!' + self.ansi_code_clear)
    quit()
  
  def load_genome(self, genome):
    self.trigger_undefined_keyword('LOAD_GENOME')
  def set_reg(self, reg_char, reg_val):
    self.trigger_undefined_keyword('SET_REG')
  def set_ip(self, val):
    self.trigger_undefined_keyword('SET_IP')
  def set_rh(self, val):
    self.trigger_undefined_keyword('SET_RH')
  def set_wh(self, val):
    self.trigger_undefined_keyword('SET_WH')
  def set_fh(self, val):
    self.trigger_undefined_keyword('SET_FH')
  def execute(self, inst_count):
    self.trigger_undefined_keyword('EXECUTE')
  def check_reg(self, reg_char, expected_reg_val):
    self.trigger_undefined_keyword('CHECK_REG')
  def check_ip(self, val):
    self.trigger_undefined_keyword('CHECK_IP')
  def check_rh(self, val):
    self.trigger_undefined_keyword('CHECK_RH')
  def check_wh(self, val):
    self.trigger_undefined_keyword('CHECK_WH')
  def check_fh(self, val):
    self.trigger_undefined_keyword('CHECK_FH')
  def check_stack_a(self, idx, val):
    self.trigger_undefined_keyword('CHECK_STACK_A')
  def check_stack_b(self, idx, val):
    self.trigger_undefined_keyword('CHECK_STACK_B')


