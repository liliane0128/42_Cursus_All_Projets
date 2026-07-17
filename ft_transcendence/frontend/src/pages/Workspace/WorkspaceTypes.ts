import { Task } from './TasksPanel';
import { FileItem } from './SharedFilesPanel';

export interface Message {
  id: string;
  user: string;
  senderId: string;
  text: string;
  createdAt: string;
  edited?: boolean;
}

export interface Workspace {
  id: number;
  name: string;
  description: string;
  deadline: string | null;
  is_completed: boolean;
  tasks: Task[];
  files: FileItem[];
  messages: Message[];
}
