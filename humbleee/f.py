try:
    user_input = input() # raw_input in Python 2.x
    if not user_input:
        raise ValueError('empty string')
except ValueError as e:
    print(e)
